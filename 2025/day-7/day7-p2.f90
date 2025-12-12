module simulation
  implicit none
  integer, parameter :: MAX_NODES = 30000
  integer(kind=8), save :: num_paths(MAX_NODES)
end module simulation

program day7_p2
  use simulation
  implicit none
  
  character(len=200) :: grid(200)
  integer :: iostat, i, nrows, ncols
  integer :: start_pos(2)
  integer :: splitters(MAX_NODES,3)
  integer :: num_splitters
  integer :: part1
  integer(kind=8) :: part2
  
  open(10, file='input.txt', status='old')
  i = 0
  do 
    i = i + 1
    read(10, '(A)', iostat=iostat) grid(i)
    if (iostat /= 0) exit
  end do
  close(10)
  nrows = i - 1
  ncols = len_trim(grid(1))
  
  print '(A,I0,A,I0)', 'Grid Area: ', nrows, 'x', ncols
  
  call find_start(grid, nrows, ncols, start_pos)
  grid(start_pos(1))(start_pos(2):start_pos(2)) = '|'
  
  call propagate_beams(grid, nrows, ncols)
  part1 = count_splitters_hit(grid, nrows, ncols)
  print '(A,I0)', 'Part 1: ', part1
  
  num_paths = 0
  call build_dag_complete(grid, nrows, ncols, splitters, num_splitters)
  part2 = count_paths_iterative(splitters, num_splitters, nrows, ncols)
  print '(A,I0)', 'Part 2: ', part2
contains

subroutine propagate_beams(grid, nrows, ncols)
  character(len=200), intent(inout) :: grid(200)
  integer, intent(in) :: nrows, ncols
  integer :: r, c
  do r = 1, nrows-1
    do c = 1, ncols
      if (grid(r)(c:c) == '|' .and. grid(r+1)(c:c) == '.') then
        grid(r+1)(c:c) = '|'
      else if (grid(r)(c:c) == '|' .and. grid(r+1)(c:c) == '^') then
        if (c > 1) grid(r+1)(c-1:c-1) = '|'
        if (c < ncols) grid(r+1)(c+1:c+1) = '|'
      end if
    end do
  end do
end subroutine

integer function count_splitters_hit(grid, nrows, ncols) result(cnt)
  character(len=200), intent(in) :: grid(200)
  integer, intent(in) :: nrows, ncols
  integer :: r, c
  cnt = 0
  do r = 1, nrows
    do c = 1, ncols
      if (grid(r)(c:c) == '^' .and. r > 1 .and. grid(r-1)(c:c) == '|') cnt = cnt + 1
    end do
  end do
end function

subroutine build_dag_complete(grid, nrows, ncols, splitters, num_splitters)
  character(len=200), intent(in) :: grid(200)
  integer, intent(in) :: nrows, ncols
  integer, intent(out) :: splitters(MAX_NODES,3), num_splitters
  integer :: r, c, ns = 0
  
  do r = 1, nrows
    do c = 1, ncols
      if (grid(r)(c:c) == '^') then
        ns = ns + 1
        splitters(ns,1) = (r-1)*ncols + c
        splitters(ns,2) = r
        splitters(ns,3) = c
      end if
    end do
  end do
  
  do c = 1, ncols
    ns = ns + 1
    splitters(ns,1) = (nrows-1)*ncols + c
    splitters(ns,2) = nrows
    splitters(ns,3) = c
  end do
  
  call sort_by_row(splitters, ns)
  num_splitters = ns
end subroutine

subroutine sort_by_row(nodes, n)
  integer, intent(inout) :: nodes(MAX_NODES,3)
  integer, intent(in) :: n
  integer :: i, j, temp(3)
  logical :: swapped
  do i = 1, n-1
    swapped = .false.
    do j = 1, n-i
      if (nodes(j,2) > nodes(j+1,2) .or. &
          (nodes(j,2) == nodes(j+1,2) .and. nodes(j,3) > nodes(j+1,3))) then
        temp(1) = nodes(j,1); temp(2) = nodes(j,2); temp(3) = nodes(j,3)
        nodes(j,1) = nodes(j+1,1); nodes(j,2) = nodes(j+1,2); nodes(j,3) = nodes(j+1,3)
        nodes(j+1,1) = temp(1); nodes(j+1,2) = temp(2); nodes(j+1,3) = temp(3)
        swapped = .true.
      end if
    end do
    if (.not. swapped) exit
  end do
end subroutine

integer(kind=8) function count_paths_iterative(splitters, num_nodes, nrows, ncols) result(total_paths)
  integer, intent(in) :: splitters(MAX_NODES,3), num_nodes, nrows, ncols
  integer :: i, j, r, c, nr, nc
  integer(kind=8) :: paths
  
  num_paths = 0
  do i = num_nodes, 1, -1
    r = splitters(i,2); c = splitters(i,3)
    if (r == nrows) then
      num_paths(i) = 1
    else
      paths = 0
      do j = i+1, num_nodes
        nr = splitters(j,2); nc = splitters(j,3)
        if (nr > r .and. (nc == c-1 .or. nc == c+1)) then
          paths = paths + num_paths(j)
        end if
      end do
      num_paths(i) = paths
    end if
  end do
  total_paths = num_paths(1)
end function

subroutine find_start(grid, nrows, ncols, start_pos)
  character(len=200), intent(in) :: grid(200)
  integer, intent(in) :: nrows, ncols
  integer, intent(out) :: start_pos(2)
  integer :: r, c
  do r = 1, nrows
    do c = 1, ncols
      if (grid(r)(c:c) == 'S') then
        start_pos = (/r, c/)
        return
      end if
    end do
  end do
end subroutine
end program day7_p2
