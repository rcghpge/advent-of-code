program day6_part2
  implicit none
  integer, parameter :: max_lines = 3000, max_len = 20000
  character(len=max_len) :: lines(max_lines)
  integer :: h, w, i, j
  logical, allocatable :: is_sep(:)
  integer(kind=8) :: part1, part2

  call read_grid(lines, h, w)

  allocate(is_sep(w))
  do j = 1, w
    is_sep(j) = .true.
    do i = 1, h
      if (lines(i)(j:j) /= ' ') then
        is_sep(j) = .false.
        exit
      end if
    end do
  end do

  part1 = 0_8
  part2 = 0_8

  block
    integer :: c_start, c_end
    c_start = 0
    do j = 1, w + 1
      if (j <= w .and. .not. is_sep(j)) then
        if (c_start == 0) c_start = j
      else
        if (c_start > 0) then
          c_end = j - 1
          call process_problem_p1(lines, h, c_start, c_end, part1)
          call process_problem_p2(lines, h, c_start, c_end, part2)
          c_start = 0
        end if
      end if
    end do
  end block

  print '(A,I0)', 'Part 1: ', part1
  print '(A,I0)', 'Part 2: ', part2

contains

  subroutine read_grid(lines, h, w)
    character(len=*), intent(inout) :: lines(:)
    integer, intent(out) :: h, w
    integer :: unit, ios, i
    character(len=max_len) :: buf

    unit = 10
    open(unit, file='input.txt', status='old', action='read')
    h = 0
    do
      read(unit, '(A)', iostat=ios) buf
      if (ios /= 0) exit
      h = h + 1
      lines(h) = buf
    end do
    close(unit)

    w = 0
    do i = 1, h
      w = max(w, len_trim(lines(i)))
    end do
    do i = 1, h
      if (len_trim(lines(i)) < w) then
        lines(i)(len_trim(lines(i))+1:w) = ' '
      end if
    end do
  end subroutine read_grid

  ! Part 1: each row in block is one integer
  subroutine process_problem_p1(lines, h, c_start, c_end, acc_total)
    character(len=*), intent(in) :: lines(:)
    integer, intent(in) :: h, c_start, c_end
    integer(kind=8), intent(inout) :: acc_total

    character(len=1) :: op
    integer :: i, pos
    character(len=100) :: field
    integer(kind=8) :: val, acc

    op = ' '
    do pos = c_start, c_end
      if (lines(h)(pos:pos) /= ' ') then
        op = lines(h)(pos:pos)
        exit
      end if
    end do
    if (op /= '+' .and. op /= '*') return

    if (op == '+') then
      acc = 0_8
    else
      acc = 1_8
    end if

    do i = 1, h-1
      field = lines(i)(c_start:c_end)
      field = adjustl(field)
      if (len_trim(field) > 0) then
        read(field, *, err=100, end=100) val
        if (op == '+') then
          acc = acc + val
        else
          acc = acc * val
        end if
      end if
100   continue
    end do

    acc_total = acc_total + acc
  end subroutine process_problem_p1

  ! Part 2: each CHARACTER COLUMN in block is one integer (top->bottom digits)
  subroutine process_problem_p2(lines, h, c_start, c_end, acc_total)
    character(len=*), intent(in) :: lines(:)
    integer, intent(in) :: h, c_start, c_end
    integer(kind=8), intent(inout) :: acc_total

    character(len=1) :: op, ch
    integer :: pos, c, r
    integer(kind=8) :: col_nums(1:c_end-c_start+1)
    integer :: col_count
    integer(kind=8) :: acc, val
    logical :: has_digit

    ! find operator in last row
    op = ' '
    do pos = c_start, c_end
      if (lines(h)(pos:pos) /= ' ') then
        op = lines(h)(pos:pos)
        exit
      end if
    end do
    if (op /= '+' .and. op /= '*') return

    col_count = 0

    ! Each column c in this block gives one number from top->bottom digits
    do c = c_start, c_end
      val = 0_8
      has_digit = .false.
      do r = 1, h-1
        ch = lines(r)(c:c)
        if (ch >= '0' .and. ch <= '9') then
          has_digit = .true.
          val = val * 10_8 + int(ichar(ch) - ichar('0'), kind=8)
        end if
      end do
      if (has_digit) then
        col_count = col_count + 1
        col_nums(col_count) = val
      end if
    end do

    if (col_count == 0) return

    if (op == '+') then
      acc = 0_8
      do c = 1, col_count
        acc = acc + col_nums(c)
      end do
    else
      acc = 1_8
      do c = 1, col_count
        acc = acc * col_nums(c)
      end do
    end if

    acc_total = acc_total + acc
  end subroutine process_problem_p2

end program day6_part2
