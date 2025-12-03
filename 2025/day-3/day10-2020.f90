program joltmeter
  implicit none
  integer, parameter :: MAX_ADAPTERS = 200
  integer :: adapters(MAX_ADAPTERS)
  integer :: n, i, j, ones, threes, diff, val, ios  ! j DECLARED
  character(len=10) :: line
  
  n = 0
  ones = 0
  threes = 0
  
  do i = 1, MAX_ADAPTERS
    read(*,'(A)',iostat=ios) line
    if(ios /= 0) exit
    if(len_trim(line) == 0) cycle
    read(line,*,iostat=ios) val
    if(ios /= 0) cycle
    n = n + 1
    adapters(n) = val
  end do
  
  do i = 1, n-1
    do j = 1, n-i
      if(adapters(j) > adapters(j+1))then
        val = adapters(j)
        adapters(j) = adapters(j+1)
        adapters(j+1) = val
      end if
    end do
  end do
  
  if(n > 0)then
    diff = adapters(1) - 0
    if(diff == 1) ones = ones + 1
    if(diff == 3) threes = threes + 1
    do i = 2, n
      diff = adapters(i) - adapters(i-1)
      if(diff == 1) ones = ones + 1
      if(diff == 3) threes = threes + 1
    end do
    threes = threes + 1
  end if
  
  print '(A,I4)', 'Total joltage difference: ', ones * threes
end program joltmeter
