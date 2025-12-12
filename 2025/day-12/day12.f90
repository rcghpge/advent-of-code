program day12
  implicit none
  integer, parameter :: MAX_SHAPES = 10, MAX_ROWS = 10, MAX_COLS = 10, MAX_GRID = 400
  integer :: nshapes = 0, count_total = 0
  integer :: shape_w(MAX_SHAPES), shape_h(MAX_SHAPES), shape_area(MAX_SHAPES)
  character(len=MAX_COLS) :: shapes(MAX_ROWS, MAX_SHAPES)
  integer :: region_w, region_h, region_counts(MAX_SHAPES)
  integer :: grid(MAX_GRID)
  character(len=100) :: line
  integer :: iostat, i
  
  open(10, file='input.txt', status='old')
  call parse_shapes(nshapes, shape_w, shape_h, shape_area, shapes)
  print *, 'Parsed', nshapes, 'shapes'
  
  rewind(10)
  call skip_to_regions
  
  count_total = 0
  do
    read(10,'(a)',iostat=iostat) line
    if (iostat /= 0) exit
    line = adjustl(line)
    if (index(line,'x') == 0 .or. index(line,':') == 0) cycle
    
    call parse_region_safe(line, region_w, region_h, region_counts, nshapes)
    print *, 'Region', region_w, 'x', region_h, 'counts:', (region_counts(i), i=1,nshapes)
    
    grid = 0
    if (quick_area_check(region_w, region_h, region_counts, nshapes, shape_area)) then
      count_total = count_total + 1
      print *, 'SUCCESS (area ok)'
    else
      print *, 'FAIL (area exceeded)'
    endif
  enddo
  
  close(10)
  print *, 'Total successful regions:', count_total

contains

subroutine parse_shapes(nshapes, shape_w, shape_h, shape_area, shapes)
  integer, intent(out) :: nshapes, shape_w(*), shape_h(*), shape_area(*)
  character(len=*), intent(out) :: shapes(:,:)
  character(len=100) :: line
  integer :: iostat, i, h, idx
  
  nshapes = 0
  
  do
    read(10,'(a)',iostat=iostat) line
    if (iostat /= 0) exit
    line = adjustl(line)
    if (index(line,':') == 0) cycle
    
    nshapes = nshapes + 1
    read(line(1:index(line,':')-1),*,iostat=iostat) idx
    
    h = 0
    shape_loop: do i = 1, MAX_ROWS
      read(10,'(a)',iostat=iostat) line
      if (iostat /= 0) exit shape_loop
      line = adjustl(line)
      if (len_trim(line) == 0 .or. index(line,':') > 0 .or. line(1:1) == ' ') exit shape_loop
      h = i
      shapes(i,nshapes) = line(1:MAX_COLS)
      shape_w(nshapes) = len_trim(line)
    enddo shape_loop
    
    shape_h(nshapes) = h
    shape_area(nshapes) = count_hash(shapes(1:h,nshapes), shape_w(nshapes), h)
    print *, 'Shape', nshapes, shape_w(nshapes), 'x', h, 'area=', shape_area(nshapes)
    
    if (index(line,':') > 0) backspace(10)
  enddo
end subroutine parse_shapes

subroutine skip_to_regions
  character(len=100) :: line
  integer :: iostat
  do
    read(10,'(a)',iostat=iostat) line
    if (iostat /= 0) exit
    if (index(line,'x') > 0 .and. index(line,':') > 0) then
      backspace(10)
      exit
    endif
  enddo
end subroutine skip_to_regions

subroutine parse_region_safe(line, w, h, counts, nshapes)
  character(len=*), intent(in) :: line
  integer, intent(out) :: w, h, counts(*)
  integer, intent(in) :: nshapes
  integer :: i, posx, pos_colon
  character(len=10) :: num_str
  
  w = 0; h = 0
  do i = 1, nshapes
    counts(i) = 0
  enddo
  
  posx = index(line,'x')
  if (posx > 0) then
    read(line(1:posx-1),*,iostat=iostat) w
    read(line(posx+1:),*,iostat=iostat) h
  endif
  
  pos_colon = index(line,':')
  if (pos_colon > 0) then
    i = 1
    parse_count: do
      if (i > nshapes) exit
      num_str = extract_next_number(line, pos_colon+1)
      if (len_trim(num_str) == 0) exit
      read(num_str,*,iostat=iostat) counts(i)
      i = i + 1
    enddo parse_count
  endif
end subroutine parse_region_safe

function extract_next_number(line, start_pos) result(num_str)
  character(len=*), intent(in) :: line
  integer, intent(in) :: start_pos
  character(len=10) :: num_str
  integer :: i, j
  
  num_str = ''
  i = start_pos
  do while (i <= len_trim(line))
    if (scan(line(i:i),'0123456789') > 0) exit
    i = i + 1
  enddo
  
  j = 0
  do while (i <= len_trim(line) .and. j < 10)
    if (scan(line(i:i),'0123456789') == 0) exit
    j = j + 1
    num_str(j:j) = line(i:i)
    i = i + 1
  enddo
end function extract_next_number

logical function quick_area_check(w, h, counts, nshapes, shape_area) result(ok)
  integer, intent(in) :: w, h, counts(*), nshapes, shape_area(*)
  integer :: total_area, i
  
  total_area = 0
  do i = 1, nshapes
    total_area = total_area + counts(i) * shape_area(i)
  enddo
  ok = (total_area <= w * h)
end function quick_area_check

pure integer function count_hash(shape, w, h) result(n)
  character(len=*), dimension(*), intent(in) :: shape
  integer, intent(in) :: w, h
  integer :: i, j
  n = 0
  do i = 1, h
    do j = 1, min(w, len_trim(shape(i)))
      if (shape(i)(j:j) == '#') n = n + 1
    enddo
  enddo
end function count_hash

end program day12
