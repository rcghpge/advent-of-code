program printing_department
  implicit none
  
  integer, parameter :: MAX_SIZE = 200
  character(len=256) :: line
  character(len=1), dimension(MAX_SIZE, MAX_SIZE) :: grid
  integer :: rows, cols, i, j, di, ni, nj
  integer :: adj_count, total_count, accessible_count, io, pos
  integer, dimension(8,2) :: directions
  
  directions(1,1) = -1; directions(1,2) = -1   ! up-left
  directions(2,1) = -1; directions(2,2) =  0   ! up
  directions(3,1) = -1; directions(3,2) =  1   ! up-right
  directions(4,1) =  0; directions(4,2) = -1   ! left
  directions(5,1) =  0; directions(5,2) =  1   ! right
  directions(6,1) =  1; directions(6,2) = -1   ! down-left
  directions(7,1) =  1; directions(7,2) =  0   ! down
  directions(8,1) =  1; directions(8,2) =  1   ! down-right
  
  rows = 0; cols = 0
  open(unit=10, file='input.txt', status='old', iostat=io)
  do
    read(10, '(A)', iostat=io) line
    if (io /= 0) exit
    pos = len_trim(line)
    if (pos > 0) then
      rows = rows + 1
      if (cols == 0) cols = pos
    end if
  end do
  close(10)
  
  print '(A,I0,A,I0)', 'Total Area: ', rows, 'x', cols
  
  open(unit=10, file='input.txt', status='old')
  do i = 1, rows
    read(10, '(A)') line
    do j = 1, cols
      grid(i,j) = line(j:j)
    end do
  end do
  close(10)
  
  total_count = 0
  accessible_count = 0
  
  do i = 1, rows
    do j = 1, cols
      if (grid(i,j) == '@') then
        total_count = total_count + 1
        adj_count = 0
        
        do di = 1, 8
          ni = i + directions(di,1)
          nj = j + directions(di,2)
          
          if (ni >= 1 .and. ni <= rows .and. nj >= 1 .and. nj <= cols) then
            if (grid(ni,nj) == '@') then
              adj_count = adj_count + 1
            end if
          end if
        end do
        
        if (adj_count < 4) accessible_count = accessible_count + 1
      end if
    end do
  end do
  
  print '(A,I5)', 'Total Inventory (paper rolls): ', total_count
  print '(A,I5)', 'Target Inventory (accessible paper rolls):', accessible_count
  print '(A,I5)', 'Total Count:', accessible_count
  
end program printing_department
