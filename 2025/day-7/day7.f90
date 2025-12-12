program day7
  implicit none
  integer, parameter :: max_rows = 3000, max_cols = 3000
  integer, parameter :: max_beams = max_rows * max_cols
  character(len=max_cols) :: grid(max_rows)
  integer :: nrows, ncols
  integer :: sr, sc, r, c
  integer :: split_count

  call read_grid(grid, nrows, ncols)
  sr = -1; sc = -1
  do r = 1, nrows
    do c = 1, ncols
      if (grid(r)(c:c) == 'S') then
        sr = r; sc = c
        exit
      end if
    end do
    if (sr /= -1) exit
  end do
  if (sr == -1) stop "No S found"

  split_count = simulate(grid, nrows, ncols, sr+1, sc)
  print '(A,I4)', 'Total Count: ', split_count

contains
  subroutine read_grid(grid, nrows, ncols)
    character(len=*), intent(inout) :: grid(:)
    integer, intent(out) :: nrows, ncols
    integer :: unit, ios, i
    character(len=max_cols) :: buf

    unit = 10
    open(unit, file='input.txt', status='old', action='read')
    nrows = 0
    do
      read(unit, '(A)', iostat=ios) buf
      if (ios /= 0) exit
      nrows = nrows + 1
      grid(nrows) = buf
    end do
    close(unit)

    ncols = 0
    do i = 1, nrows
      ncols = max(ncols, len_trim(grid(i)))
    end do
    do i = 1, nrows
      if (len_trim(grid(i)) < ncols) then
        grid(i)(len_trim(grid(i))+1:ncols) = ' '
      end if
    end do
  end subroutine read_grid

  function simulate(grid, nrows, ncols, r0, c0) result(splits)
    character(len=*), intent(in) :: grid(:)
    integer, intent(in) :: nrows, ncols, r0, c0
    integer :: splits
    integer :: q_row(max_beams), q_col(max_beams)
    logical :: split_seen(max_rows, max_cols)
    integer :: head, tail
    integer :: br, bc

    splits = 0
    head   = 1
    tail   = 0
    split_seen = .false.

    if (r0 >= 1 .and. r0 <= nrows .and. c0 >= 1 .and. c0 <= ncols) then
      tail       = 1
      q_row(1)   = r0
      q_col(1)   = c0
    end if

    do while (head <= tail)
      br = q_row(head)
      bc = q_col(head)
      head = head + 1

      if (br < 1 .or. br > nrows .or. bc < 1 .or. bc > ncols) cycle

      if (grid(br)(bc:bc) == '^') then
        if (.not. split_seen(br,bc)) then
          split_seen(br,bc) = .true.
          splits = splits + 1

          if (tail + 1 <= max_beams) then
            tail = tail + 1
            q_row(tail) = br
            q_col(tail) = bc - 1
          end if

          if (tail + 1 <= max_beams) then
            tail = tail + 1
            q_row(tail) = br
            q_col(tail) = bc + 1
          end if
        end if

      else
        if (tail + 1 <= max_beams) then
          tail = tail + 1
          q_row(tail) = br + 1
          q_col(tail) = bc
        end if
      end if
    end do
  end function simulate
  
end program day7
