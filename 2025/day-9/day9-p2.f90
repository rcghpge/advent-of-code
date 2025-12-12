program day9_p2
  implicit none
  integer, parameter :: MAX_POINTS = 2000
  integer, parameter :: MAXW = 2000, MAXH = 2000

  integer(kind=8) :: x(MAX_POINTS), y(MAX_POINTS)
  integer :: n_points, i, j, k, io
  character(len=40) :: line
  integer :: minx, maxx, miny, maxy
  integer :: w, h
  integer :: gx, gy
  integer :: grid(0:MAXW,0:MAXH)

  integer(kind=8) :: xa, ya, xb, yb
  integer(kind=8) :: dx, dy, area, best
  logical :: ok

  integer :: qx(0:MAXW*MAXH), qy(0:MAXW*MAXH)
  integer :: head, tail

  n_points = 0
  do
    read(*,'(A)',iostat=io) line
    if (io /= 0) exit
    if (len_trim(line) == 0) cycle
    n_points = n_points + 1
    read(line,*) x(n_points), y(n_points)
  end do

  print '(A,I0,A)', 'Found ', n_points, ' red tiles'

  head = 0
  tail = 0

end program day9_p2
