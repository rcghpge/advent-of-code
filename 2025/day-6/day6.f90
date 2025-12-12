program day6
  implicit none
  integer, parameter :: max_rows = 3000, max_cols = 3000
  integer           :: nrows, ncols, i, j
  integer(kind=8)   :: nums(max_rows, max_cols)
  character(len=1)  :: ops(max_cols)
  character(len=20000) :: line
  integer(kind=8)   :: value, grand_total
  integer           :: unit, ios

  unit = 10
  open(unit, file='input.txt', status='old', action='read')

  nrows = 0
  do
    read(unit, '(A)', iostat=ios) line
    if (ios /= 0) exit
    nrows = nrows + 1
    call strip_trailing(line)
    call parse_numbers(line, nums, nrows, ncols)
  end do
  close(unit)

  call parse_ops(line, ops, ncols)
  nrows = nrows - 1

  grand_total = 0_8

  do j = 1, ncols
    if (ops(j) == '+') then
      value = 0_8
      do i = 1, nrows
        value = value + nums(i, j)
      end do
    else if (ops(j) == '*') then
      value = 1_8
      do i = 1, nrows
        value = value * nums(i, j)
      end do
    else
      cycle
    end if
    grand_total = grand_total + value
  end do

  print '(A,I0)', 'Grand Total: ', grand_total

contains

  subroutine strip_trailing(s)
    character(len=*), intent(inout) :: s
    integer :: k
    do k = len(s), 1, -1
      if (s(k:k) /= ' ') then
        if (k < len(s)) s(k+1:) = ' '
        return
      end if
    end do
  end subroutine strip_trailing

  subroutine parse_numbers(s, nums, row, ncols)
    character(len=*), intent(in)    :: s
    integer(kind=8), intent(inout)  :: nums(:,:)
    integer, intent(in)             :: row
    integer, intent(inout)          :: ncols

    integer :: count, stat, pos, len_s
    character(len=20000) :: tmp

    tmp   = adjustl(s)
    len_s = len_trim(tmp)
    pos   = 1
    count = 0

    do
      if (pos > len_s) exit
      count = count + 1
      read(tmp(pos:), *, iostat=stat) nums(row, count)
      if (stat /= 0) then
        count = count - 1
        exit
      end if
      call advance_field(tmp, pos)
    end do

    if (row == 1) ncols = count
  end subroutine parse_numbers

  subroutine advance_field(s, pos)
    character(len=*), intent(in) :: s
    integer, intent(inout)       :: pos
    integer :: l

    l = len_trim(s)
    do while (pos <= l .and. s(pos:pos) /= ' ')
      pos = pos + 1
    end do
    do while (pos <= l .and. s(pos:pos) == ' ')
      pos = pos + 1
    end do
  end subroutine advance_field

  subroutine parse_ops(s, ops, ncols)
    character(len=*), intent(in) :: s
    character(len=1), intent(out):: ops(:)
    integer, intent(in)          :: ncols
    integer :: i, count

    count = 0
    do i = 1, len_trim(s)
      if (s(i:i) == '+' .or. s(i:i) == '*') then
        count      = count + 1
        ops(count) = s(i:i)
        if (count == ncols) exit
      end if
    end do
  end subroutine parse_ops

end program day6
