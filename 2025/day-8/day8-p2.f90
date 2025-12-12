program day8_p2
  implicit none
  
  integer, parameter :: MAXN = 2000
  integer, parameter :: MAXE = 2000*1999/2
  
  integer :: n = 0
  real(8) :: points(MAXN, 3)
  real(8) :: edges(MAXE, 3)
  integer :: parent(MAXN)
  integer :: rankk(MAXN)
  integer :: sizes(MAXN)
  integer :: last_i = -1, last_j = -1
  
  integer :: i, j, k, e_count = 0
  real(8) :: x1, y1, z1, x2, y2, z2, dist
  
  write(*, '(A)') 'Reading input...'
  call read_points(points, n)
  write(*, '(A,I0,A)') 'Read ', n, ' points'

  write(*, '(A)') ''
  write(*, '(A)') 'Generating edges...'
  e_count = 0
  do i = 1, n
    do j = i+1, n
      x1 = points(i,1); y1 = points(i,2); z1 = points(i,3)
      x2 = points(j,1); y2 = points(j,2); z2 = points(j,3)
      dist = sqrt( (x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2 )
      e_count = e_count + 1
      if (e_count > MAXE) stop 'Too many edges!'
      edges(e_count, 1) = dist
      edges(e_count, 2) = i
      edges(e_count, 3) = j
    end do
  end do
  write(*, '(A,I0,A)') 'Generated ', e_count, ' edges'
  
  write(*, '(A)') 'Sorting edges...'
  call quicksort_edges(edges, 1, e_count)
  
  write(*,*) ''
  write(*,*) '=== Part 1: Processing 1000 shortest edges ==='
  do i = 1, n
    parent(i) = i
    rankk(i) = 0
    sizes(i) = 1
  end do
  do k = 1, min(1000, e_count)
    i = nint(edges(k,2))
    j = nint(edges(k,3))
    call union_find(i, j, parent, rankk, sizes, n)
  end do
  call find_largest_three(sizes, parent, n)
  
  write(*,*) ''
  write(*,*) '=== Part 2: Connecting until single circuit ==='
  do i = 1, n
    parent(i) = i
    rankk(i) = 0
    sizes(i) = 1
  end do
  
  last_i = -1; last_j = -1
  do k = 1, e_count
    i = nint(edges(k,2))
    j = nint(edges(k,3))
    if (find(i, parent) /= find(j, parent)) then  
      call union_find(i, j, parent, rankk, sizes, n)
      last_i = i
      last_j = j
 
      if (sizes(find(1, parent)) == n) then
        write(*, '(A,I0)') 'Part 2: Single circuit formed at edge ', k
        write(*, '(A,I0,A,I0)') 'Last connection: boxes ', last_i+1, ' and ', last_j+1
        write(*, '(A,I0,A,I0)') 'X coordinates: ', nint(points(last_i,1)), ' * ', nint(points(last_j,1))
        write(*, '(A,I0,I0)') 'Part 2 Answer: ', nint(points(last_i,1)) * nint(points(last_j,1))
        exit
      end if
    end if
  end do
  
contains

subroutine read_points(points, n)
  implicit none
  real(8), intent(inout) :: points(:,:)
  integer, intent(out) :: n
  integer :: i, io
  character(256) :: line
  integer :: comma1, comma2
  
  n = 0
  open(10, file='input.txt', status='old', iostat=io)
  if (io /= 0) stop 'Cannot open input.txt'
  
  do i = 1, size(points,1)
    read(10, '(A)', iostat=io) line
    if (io /= 0) exit
    line = trim(adjustl(line))
    if (len_trim(line) == 0) cycle
    
    comma1 = index(line, ',')
    if (comma1 == 0) cycle
    comma2 = index(line(comma1+1:), ',') + comma1
    if (comma2 == comma1) cycle
    
    read(line(1:comma1-1), *, iostat=io) points(i,1)
    if (io /= 0) cycle
    read(line(comma1+1:comma2-1), *, iostat=io) points(i,2)
    if (io /= 0) cycle
    read(line(comma2+1:), *, iostat=io) points(i,3)
    if (io /= 0) cycle
    n = i
  end do
  close(10)
end subroutine

recursive integer function find(x, parent) result(res)
  implicit none
  integer, intent(in) :: x
  integer, intent(inout) :: parent(:)
  if (parent(x) /= x) then
    res = find(parent(x), parent)
    parent(x) = res
  else
    res = x
  end if
end function find

subroutine union_find(x, y, parent, rankk, sizes, n)
  implicit none
  integer, intent(in) :: x, y, n
  integer, intent(inout) :: parent(:), rankk(:), sizes(:)
  integer :: px, py
  
  px = find(x, parent)
  py = find(y, parent)
  if (px == py) return
  
  if (rankk(px) < rankk(py)) then
    parent(px) = py
    sizes(py) = sizes(py) + sizes(px)
  else
    parent(py) = px
    sizes(px) = sizes(px) + sizes(py)
    if (rankk(px) == rankk(py)) rankk(px) = rankk(px) + 1
  end if
end subroutine union_find

recursive subroutine quicksort_edges(arr, left, right)
  implicit none
  real(8), intent(inout) :: arr(:,:)
  integer, intent(in) :: left, right
  integer :: i, j, pivot_idx
  real(8) :: pivot, temp(3)
  
  if (left >= right) return
  
  pivot_idx = (left + right) / 2
  pivot = arr(pivot_idx, 1)
  temp = arr(pivot_idx, :)
  arr(pivot_idx, :) = arr(right, :)
  arr(right, :) = temp
  
  i = left
  j = right - 1
  
  do
    do while (i <= j .and. arr(i,1) < pivot)
      i = i + 1
    end do
    do while (j >= i .and. arr(j,1) > pivot)
      j = j - 1
    end do
    if (i >= j) exit
    temp = arr(i,:)
    arr(i,:) = arr(j,:)
    arr(j,:) = temp
    i = i + 1
    j = j - 1
  end do
  
  temp = arr(i,:)
  arr(i,:) = arr(right,:)
  arr(right,:) = temp
  
  call quicksort_edges(arr, left, i-1)
  call quicksort_edges(arr, i+1, right)
end subroutine quicksort_edges

subroutine find_largest_three(sizes, parent, n)
  implicit none
  integer, intent(in) :: sizes(:), n
  integer, intent(inout) :: parent(:)
  integer :: i, root
  integer :: counts(MAXN), largest(3)
  
  counts = 0
  do i = 1, n
    root = find(i, parent)
    counts(root) = sizes(root)
  end do
  
  largest = 0
  do i = 1, n
    if (counts(i) > 0) then
      if (counts(i) > largest(1)) then
        largest(3) = largest(2)
        largest(2) = largest(1)
        largest(1) = counts(i)
      else if (counts(i) > largest(2)) then
        largest(3) = largest(2)
        largest(2) = counts(i)
      else if (counts(i) > largest(3)) then
        largest(3) = counts(i)
      end if
    end if
  end do
  
  write(*, '(A,I0,A,I0,A,I0)') 'Part 1 - Three largest: ', largest(1),',', largest(2), ',', largest(3)
  write(*, '(A,I0,I0,I0)') 'Part 1 - Product: ', largest(1)*largest(2)*largest(3)
end subroutine find_largest_three
end program day8_p2
