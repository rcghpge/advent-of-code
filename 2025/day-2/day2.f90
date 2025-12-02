program gift_shop_dbms
  implicit none
  integer, parameter :: max_ranges = 100
  character(len=2000) :: line
  character(len=30) :: token
  integer :: i, count_ranges, ios, pos, comma_pos
  integer(8), dimension(max_ranges, 2) :: ranges
  integer(8) :: start_id_8, end_id_8
  integer(8) :: j, invalid_sum
  character(len=30) :: id_str, first_half, second_half
  integer :: length, half_len
  character(len=:), allocatable :: rest_line

  open(unit=10, file='input.txt', status='old', action='read', iostat=ios)
  if (ios /= 0) then
    print *, 'Error opening input data'
    stop
  end if
  read(10, '(A)', iostat=ios) line
  if (ios /= 0) then
    print *, 'Error reading from input data'
    stop
  end if
  close(10)

  line = adjustl(trim(line))
  count_ranges = 0
  rest_line = line

  do while(len_trim(rest_line) > 0)
     comma_pos = index(rest_line, ',')
     if (comma_pos == 0) then
        token = trim(rest_line)
        rest_line = ""
     else
        token = rest_line(1:comma_pos-1)
        rest_line = rest_line(comma_pos+1:)
     end if

     count_ranges = count_ranges + 1
     if (count_ranges > max_ranges) then
        print *, 'Error: too many ranges. Increase max_ranges.'
        stop
     end if

     pos = index(token, '-')
     if (pos == 0) then
        print *, 'Invalid range format: ', trim(token)
        stop
     end if

     read(token(1:pos-1), '(I20)', iostat=ios) start_id_8
     if (ios /= 0) then
        print *, 'Invalid start product ID in range: ', trim(token)
        stop
     end if

     read(token(pos+1:len_trim(token)), '(I20)', iostat=ios) end_id_8
     if (ios /= 0) then
        print *, 'Invalid end product ID in range: ', trim(token)
        stop
     end if

     ranges(count_ranges,1) = start_id_8
     ranges(count_ranges,2) = end_id_8
  end do

  invalid_sum = 0_8

  do i = 1, count_ranges
     do j = ranges(i, 1), ranges(i, 2)
        write(id_str, '(I30)') j
        id_str = adjustl(trim(id_str))
        length = len_trim(id_str)
        if (mod(length, 2) == 0) then
           half_len = length / 2
           first_half = id_str(1:half_len)
           second_half = id_str(half_len+1:length)
           if ((first_half == second_half) .and. first_half(1:1) /= '0') then
              invalid_sum = invalid_sum + j
           end if
        end if
     end do
  end do
  print '(A,I10)', 'Sum of invalid product IDs: ', invalid_sum

end program gift_shop_dbms
