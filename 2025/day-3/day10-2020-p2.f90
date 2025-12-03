program joltage_battery_tester
  implicit none
  integer, parameter :: MAX_ADAPTERS = 200
  integer :: adapters(MAX_ADAPTERS+2)
  integer :: n, i, j, ios, val
  integer(kind=8) :: dp(MAX_ADAPTERS+2)
  character(len=10) :: line
  
  n = 0
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
    do j = i+1, n
      if(adapters(i) > adapters(j))then
        val = adapters(i)
        adapters(i) = adapters(j)
        adapters(j) = val
      end if
    end do
  end do
  
  do i = n, 1, -1
    adapters(i+1) = adapters(i)
  end do
  adapters(1) = 0           
  adapters(n+2) = adapters(n+1) + 3  
  n = n + 2
  
  dp(1) = 1 
  
  do i = 2, n
    dp(i) = 0
    do j = max(1, i-3), i-1
      if(adapters(i) - adapters(j) <= 3) dp(i) = dp(i) + dp(j)
    end do
  end do
  
print '(A,I15)', 'Total # of battery/adapter arrangements:', dp(n)
end program joltage_battery_tester
