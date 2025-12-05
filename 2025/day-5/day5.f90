program cafeteria_dbms
  implicit none
  integer, parameter :: lu = 10, kind8 = selected_int_kind(18)
  integer :: ios, n_ranges = 0, n_avail = 0, i, j, pos
  integer(kind=kind8) :: id, low, high, count = 0
  character(len=256) :: line
  integer(kind=kind8), allocatable :: ranges(:,:), avail_ids(:)
  
  open(unit=lu, file='input.txt', status='old', action='read')
  do
    read(lu, '(A)', iostat=ios) line
    if (ios /= 0 .or. len_trim(line) == 0) exit
    n_ranges = n_ranges + 1
  end do
  
  allocate(ranges(n_ranges, 2))
  rewind(lu)
  do i = 1, n_ranges
    read(lu, '(A)') line
    pos = index(line, '-')
    read(line(1:pos-1), *) low
    read(line(pos+1:), *) high
    ranges(i, 1) = low
    ranges(i, 2) = high
  end do
  
  read(lu, '(A)') line
  
  do
    read(lu, '(A)', iostat=ios) line
    if (ios /= 0) exit
    if (len_trim(line) == 0) cycle
    n_avail = n_avail + 1
  end do
  
  allocate(avail_ids(n_avail))
  rewind(lu)
  do i = 1, n_ranges    
    read(lu, '(A)')
  end do
  read(lu, '(A)') line 
  
  do i = 1, n_avail
    read(lu, '(A)') line
    read(line, *) avail_ids(i)
  end do
  
  count = 0
  do i = 1, n_avail
    id = avail_ids(i)
    do j = 1, n_ranges
      if (id >= ranges(j,1) .and. id <= ranges(j,2)) then
        count = count + 1
        exit
      end if
    end do
  end do

  print '(A)', "Cafeteria DBMS"
  print '(A,I0)', "Total Count (fresh ingredients): ", count
  close(lu)
end program cafeteria_dbms
