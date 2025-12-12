module solver
  implicit none
contains

subroutine parse_machine(line, n_lights, target, n_buttons, buttons)
  character(len=*), intent(in) :: line
  integer, intent(out) :: n_lights, n_buttons
  integer, allocatable, intent(out) :: target(:), buttons(:,:)
  integer :: i, j, pos1, pos2, start_pos, end_pos
  character(len=200) :: clean_line

  clean_line = trim(adjustl(line))
  pos1 = index(clean_line, '[') + 1
  pos2 = index(clean_line, ']')
  if (pos1 == 0 .or. pos2 == 0) then
    n_lights = 0
    n_buttons = 0
    return
  end if

  n_lights = pos2 - pos1
  allocate(target(n_lights))

  do i = 1, n_lights
    if (clean_line(pos1+i-1:pos1+i-1) == '#') then
      target(i) = 1
    else
      target(i) = 0
    end if
  end do

  n_buttons = 0
  do i = 1, len_trim(clean_line)
    if (clean_line(i:i) == '(') n_buttons = n_buttons + 1
  end do

  if (n_buttons == 0) then
    allocate(buttons(0,0))
    return
  end if

  allocate(buttons(n_lights, n_buttons))
  buttons = 0

  j = 0
  do i = 1, len_trim(clean_line)
    if (clean_line(i:i) == '(') then
      j = j + 1
      start_pos = i + 1
    else if (clean_line(i:i) == ')') then
      end_pos = i - 1
      call parse_button(clean_line(start_pos:end_pos), n_lights, buttons(:,j))
    end if
  end do
end subroutine parse_machine

subroutine parse_button(btn_str, n_lights, button_vec)
  character(len=*), intent(in) :: btn_str
  integer, intent(in) :: n_lights
  integer, intent(inout) :: button_vec(n_lights)
  integer :: i, num_start, num_end, num
  character(len=8) :: token

  button_vec = 0
  i = 1
  do while (i <= len_trim(btn_str))
    do while (i <= len_trim(btn_str) .and. (btn_str(i:i) == ' ' .or. btn_str(i:i) == ','))
      i = i + 1
    end do
    if (i > len_trim(btn_str)) exit

    num_start = i
    do while (i <= len_trim(btn_str) .and. scan(btn_str(i:i), '0123456789') > 0)
      i = i + 1
    end do
    num_end = i - 1

    if (num_end >= num_start) then
      token = '        '
      token(1:num_end-num_start+1) = btn_str(num_start:num_end)
      read(token, *, err=999) num
      if (num >= 0 .and. num < n_lights) button_vec(num + 1) = 1
    end if
  end do
  return
999 continue
end subroutine parse_button

integer function hamming_weight(state, n_bits) result(weight)
  integer, intent(in) :: state, n_bits
  integer :: i

  weight = 0
  do i = 0, n_bits-1
    if (btest(state, i)) weight = weight + 1
  end do
end function hamming_weight

integer function solve_puzzle(line, machine_num) result(min_presses)
  character(len=*), intent(in) :: line
  integer, intent(in) :: machine_num

  integer :: n_lights, n_buttons
  integer, allocatable :: target(:), buttons(:,:)
  integer :: state, max_state, best_presses
  integer :: i, b, light_result

  call parse_machine(line, n_lights, target, n_buttons, buttons)

  if (n_lights == 0 .or. n_buttons == 0) then
    min_presses = 0
    return
  end if

  max_state    = 2**n_buttons
  best_presses = 999

  do state = 0, max_state-1
    if (hamming_weight(state, n_buttons) >= best_presses) cycle

    do i = 1, n_lights
      light_result = 0
      do b = 1, n_buttons
        if (btest(state, b-1) .and. buttons(i,b) == 1) then
          light_result = ieor(light_result, 1)
        end if
      end do

      if (light_result /= target(i)) exit
    end do

    if (i > n_lights) then
      best_presses = hamming_weight(state, n_buttons)
      if (best_presses == 0) exit
    end if
  end do

  if (best_presses == 999) then
    min_presses = 0
  else
    min_presses = best_presses
  end if

  if (allocated(target))  deallocate(target)
  if (allocated(buttons)) deallocate(buttons)
end function solve_puzzle

end module solver

program day10
  use solver
  implicit none

  character(len=200) :: line
  integer :: total_presses, io, machine_num, presses

  total_presses = 0
  machine_num   = 0

  do
    read(*, '(A)', iostat=io) line
    if (io /= 0) exit
    if (len_trim(line) == 0) cycle

    machine_num = machine_num + 1
    presses = solve_puzzle(trim(line), machine_num)
    total_presses = total_presses + presses
    print '(A,I0,A,I0)', 'Machine ', machine_num, ': ', presses
  end do

  print '(A,I0)', 'Total presses: ', total_presses
end program day10
