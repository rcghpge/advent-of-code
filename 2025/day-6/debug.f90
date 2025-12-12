program day6_p1
  implicit none
  character(len=1000), allocatable :: lines(:)
  integer :: i, j, h, w, grand_total
  logical, allocatable :: is_separator(:)
  
  call read_input_lines(lines)
  h = size(lines)
  if (h == 0) stop "No input"
  
  ! Pad all lines to same length with spaces
  w = 0
  do i = 1, h
    w = max(w, len_trim(lines(i)))
  end do
  do i = 1, h
    if (len_trim(lines(i)) < w) then
      lines(i) = trim(lines(i)) // repeat(' ', w - len_trim(lines(i)))
    end if
  end do
  
  allocate(is_separator(w))
  
  ! Separator = ALL rows INCLUDING operator row are spaces
  do j = 1, w
    is_separator(j) = .true.
    do i = 1, h
      if (lines(i)(j:j) /= ' ') then
        is_separator(j) = .false.
        exit
      end if
    end do
  end do
  
  grand_total = 0
  
  ! Process each problem block
  block
    integer :: c_start, c_end
    c_start = 0
    do j = 1, w + 1
      if (j <= w .and. .not. is_separator(j)) then
        if (c_start == 0) c_start = j
      else
        if (c_start > 0) then
          c_end = j - 1
          call process_block(lines, h, w, c_start, c_end, grand_total)
          c_start = 0
        end if
      end if
    end do
  end block
  
  print *, 'Grand total:', grand_total
  
contains

subroutine read_input_lines(lines)
  character(len=1000), allocatable, intent(out) :: lines(:)
  character(len=1000) :: buffer
  integer :: unit = 10, ios, nlines, i
  
  open(unit, file='input.txt', status='old')
  nlines = 0
  do
    read(unit, '(A)', iostat=ios) buffer
    if (ios /= 0) exit
    nlines = nlines + 1
  end do
  rewind(unit)
  allocate(lines(nlines))
  do i = 1, nlines
    read(unit, '(A)') lines(i)
  end do
  close(unit)
end subroutine

subroutine process_block(lines, h, w, c_start, c_end, grand_total)
  character(len=*), intent(in) :: lines(:)
  integer, intent(in) :: h, w, c_start, c_end
  integer, intent(inout) :: grand_total
  integer :: i, j, result, num
  character(len=1) :: op
  character(len=100) :: num_str
  
  ! Find operator in LAST row (h)
  op = ' '
  do j = c_start, c_end
    if (lines(h)(j:j) /= ' ') then
      op = lines(h)(j:j)
      exit
    end if
  end do
  
  if (op /= '+' .and. op /= '*') return
  
  ! For each ROW 1..h-1, extract ONE number from columns c_start..c_end
  result = merge(1, 0, op == '*')
  do i = 1, h-1
    num_str = trim(adjustl(lines(i)(c_start:c_end)))
    if (len_trim(num_str) > 0) then
      read(num_str, *) num
      if (op == '+') then
        result = result + num
      else
        result = result * num
      end if
    end if
  end do
  
  grand_total = grand_total + result
  print *, 'Block', c_start, '-', c_end, 'op=', op, 'result=', result  ! DEBUG
end subroutine

end program day6_p1
