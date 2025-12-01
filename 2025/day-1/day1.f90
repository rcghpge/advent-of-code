program day1
  implicit none
  
  integer :: position, zero_count, iostat_val, distance
  character(len=100) :: line
  character :: direction
  
  position = 50
  zero_count = 0
  
  print *, 'Advent of Code 2025 - Day 1: Secret Entrance'
  print *, 'Starting position: ', position
  print *
  
  do
    read(*,'(A)', iostat=iostat_val) line
    if (iostat_val /= 0) exit 
    
    if (len_trim(line) < 2) cycle
    
    direction = line(1:1)
    read(line(2:), *) distance
    
    if (direction == 'R') then
      position = mod(position + distance, 100)
    else if (direction == 'L') then
      position = mod(position - distance + 10000, 100)
    end if
    
    write(*, '(A,A,A,I2)') 'After ', trim(line), ': position = ', position
    if (position == 0) then
      zero_count = zero_count + 1
      print '(A,I0,A)', '-> PASSED 0! (count = ', zero_count, ')'
    end if
    print *
  end do
  
  print *, '============================================'
  print '(A,I0,A)', 'The actual password is: ', zero_count 
  print *, '============================================'

end program day1
