program printing_departmentv2
  implicit none

  integer, parameter :: MAX_SIZE = 200
  character(len=256) :: line
  character(len=1), dimension(MAX_SIZE, MAX_SIZE) :: grid
  integer :: rows, cols, i, j, di, ni, nj
  integer :: adj_count, total_removed, removed_this_round, io, pos
  integer, dimension(8,2) :: directions
  logical :: changed
  character(len=1), dimension(MAX_SIZE, MAX_SIZE) :: new_grid

  directions(1,:) = [-1,-1]
  directions(2,:) = [-1, 0]
  directions(3,:) = [-1, 1]
  directions(4,:) = [ 0,-1]
  directions(5,:) = [ 0, 1]
  directions(6,:) = [ 1,-1]
  directions(7,:) = [ 1, 0]
  directions(8,:) = [ 1, 1]

  rows = 0
  open(unit=10, file='input.txt', status='old', iostat=io)
  do
    read(10, '(A)', iostat=io) line
    if (io /= 0) exit
    pos = len_trim(line)
    if (pos > 0) then
      rows = rows + 1
      if (rows > MAX_SIZE) stop 'Maximum area limit reached'
      if (rows == 1) cols = pos
    end if
  end do
  close(10)

  open(unit=10, file='input.txt', status='old')
  do i = 1, rows
    read(10, '(A)') line
    do j = 1, cols
      grid(i,j) = line(j:j)
    end do
  end do
  close(10)

  total_removed = 0

  do
    changed = .false.
    removed_this_round = 0

    new_grid = grid

    do i = 1, rows
      do j = 1, cols
        if (grid(i,j) == '@') then
          adj_count = 0
          do di = 1, 8
            ni = i + directions(di,1)
            nj = j + directions(di,2)
            if (ni >= 1 .and. ni <= rows .and. nj >= 1 .and. nj <= cols) then
              if (grid(ni,nj) == '@') adj_count = adj_count + 1
            end if
          end do
          if (adj_count < 4) then
            new_grid(i,j) = '.' 
            removed_this_round = removed_this_round + 1
            changed = .true.
          end if
        end if
      end do
    end do

    grid = new_grid

    total_removed = total_removed + removed_this_round

    if (.not. changed) exit
  end do

  print '(A,I4)', "Total Inventory Count (paper rolls removed): ", total_removed

end program printing_departmentv2
