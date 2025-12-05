program cafeteria_dbmsv2
  implicit none
  integer, parameter :: lu = 10, kind8 = selected_int_kind(18)
  integer :: ios, n_ranges = 0, i, j, pos
  integer(kind=kind8) :: low, high, total = 0
  character(len=256) :: line
  integer(kind=kind8), allocatable :: starts(:), ends(:)
  
  open(unit=lu, file='input.txt', status='old', action='read')
  do
    read(lu, '(A)', iostat=ios) line
    if (ios /= 0 .or. len_trim(line) == 0) exit
    n_ranges = n_ranges + 1
  end do
  
  allocate(starts(n_ranges), ends(n_ranges))
  rewind(lu)
  do i = 1, n_ranges
    read(lu, '(A)') line
    pos = index(line, '-')
    read(line(1:pos-1), *) starts(i)
    read(line(pos+1:), *) ends(i)
  end do
  
  call sort_ranges(starts, ends, n_ranges)
  call merge_ranges(starts, ends, n_ranges, total)

  print '(A)', "Cafeteria DBMS2"
  print '(A,I0)', "Total Inventory Count (fresh ingredients by ID): ", total
  close(lu)
  
contains
  
  subroutine sort_ranges(starts, ends, n)
    integer(kind=kind8), intent(inout) :: starts(:), ends(:)
    integer, intent(in) :: n
    integer :: i, j
    integer(kind=kind8) :: temp_start, temp_end
    
    do i = 1, n-1
      do j = 1, n-i
        if (starts(j) > starts(j+1)) then
          temp_start = starts(j)
          temp_end = ends(j)
          starts(j) = starts(j+1)
          ends(j) = ends(j+1)
          starts(j+1) = temp_start
          ends(j+1) = temp_end
        end if
      end do
    end do
  end subroutine sort_ranges
  
  subroutine merge_ranges(starts, ends, n, total)
    integer(kind=kind8), intent(in) :: starts(:), ends(:)
    integer, intent(in) :: n
    integer(kind=kind8), intent(out) :: total
    integer :: i
    integer(kind=kind8) :: curr_start, curr_end
    
    if (n == 0) return
    
    curr_start = starts(1)
    curr_end = ends(1)
    
    do i = 2, n
      if (starts(i) <= curr_end + 1) then 
        curr_end = max(curr_end, ends(i))
      else
        total = total + curr_end - curr_start + 1
        curr_start = starts(i)
        curr_end = ends(i)
      end if
    end do
    total = total + curr_end - curr_start + 1
  end subroutine merge_ranges
  
end program cafeteria_dbmsv2
