program day9_p2
  implicit none
  
  integer, parameter :: MAX_POINTS = 10000
  integer(kind=8) :: n_points = 0  ! Use 64-bit for large counts
  integer(kind=8) :: x(MAX_POINTS), y(MAX_POINTS)
  integer(kind=8) :: i, j, max_area = 0
  integer :: io
  character(len=20) :: line
  integer(kind=8) :: x1, y1, dx, dy, area
  
  do
    read(*, '(a)', iostat=io) line
    if (io /= 0) exit
    if (len_trim(line) == 0) cycle
    
    read(line, *, err=10) x1, y1
    n_points = n_points + 1
    if (n_points > MAX_POINTS) stop 'Too many points!'
    x(n_points) = x1
    y(n_points) = y1
    cycle
10  continue
  end do
  print *, 'Found', n_points, 'red tiles'
  
  do i = 1, n_points
    do j = i+1, n_points
      dx = abs(x(j) - x(i)) + 1
      dy = abs(y(j) - y(i)) + 1
      area = dx * dy
      
      if (area > max_area) then
        max_area = area
        if (modulo(nint(real(i)*j/1000000), 100) == 0) &
          print '(A,I0,A,I0)', 'Progress: i = ', i, ' max_area = ', max_area
      end if
    end do
  end do
  print *, ''
  print '(A,I0,A)', '*** LARGEST RECTANGLE AREA: ', max_area, ' ***'
  
end program day9_p2
