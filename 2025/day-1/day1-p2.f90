program day1_p2
  implicit none
  
  integer :: position, zero_count, iostat_val, distance, clicks
  character(len=100) :: line
  character :: direction
  
  position = 50
  zero_count = 0
  
  print *, 'Advent of Code 2025 - Day 1 Secret Entrance Part 2 '
  print *, 'Starting position: ', position
  print *
  
  do
    read(*,'(A)', iostat=iostat_val) line
    if (iostat_val /= 0) exit
    if (len_trim(line) < 2) cycle
    
    direction = line(1:1)
    read(line(2:), *) distance
    
    if (direction == 'R') then
      do clicks = 1, distance
        if (mod((position + clicks - 1), 100) == 0) zero_count = zero_count + 1
      end do
      position = mod(position + distance, 100)
    else if (direction == 'L') then
      do clicks = 1, distance
        if (mod((position - clicks + 1 + 10000), 100) == 0) zero_count = zero_count + 1
      end do
      position = mod(position - distance + 10000, 100)
    end if
    print '(A,A,A,I3,A,I5,A)', 'After ', trim(line), ': position = ', position, &
                            ' (total passes = ', zero_count,')'
  end do
 
  print *
  print *, '============================================'
  print '(A,I0,A)', 'The actual password (method 0x434C49434B) is: ', zero_count
  print *, '============================================'

end program day1_p2
