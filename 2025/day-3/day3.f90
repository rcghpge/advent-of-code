program day3
  implicit none
  character(len=100) :: line
  integer :: maxjolt, i, j, d1, d2, total
  total = 0
  
  do
    read(*,'(A)',end=10) line
    maxjolt = 0
    do i = 1, len_trim(line)-1
      read(line(i:i),'(I1)') d1
      do j = i+1, len_trim(line)
        read(line(j:j),'(I1)') d2
        if (10*d1 + d2 > maxjolt) maxjolt = 10*d1 + d2
      end do
    end do
    total = total + maxjolt
    print '(A,I2)', 'Total max joltage: ', maxjolt  
  end do
10 print '(A,I5)', 'Total output joltage: ', total
end program day3
