program day11_p2
    
    implicit none
    
    integer, parameter :: MAX_NODES = 1100
    integer, parameter :: MAX_OUTPUTS = 50
    integer, parameter :: MAX_MEMO = 50000
    
    type :: Node
        character(len=10) :: name
        integer :: num_outputs
        character(len=10) :: outputs(MAX_OUTPUTS)
    end type Node
    
    type :: MemoEntry
        character(len=100) :: key
        integer(kind=8) :: value
        logical :: exists
    end type MemoEntry
    
    type(Node) :: graph(MAX_NODES)
    integer :: num_nodes
    integer :: svr_idx, out_idx, dac_idx, fft_idx
    type(MemoEntry) :: memo(MAX_MEMO)
    integer :: memo_count
    integer(kind=8) :: result
    
    call parse_input('input.txt')
    
    if (find_node_index('out') == 0) then
        num_nodes = num_nodes + 1
        graph(num_nodes)%name = 'out'
        graph(num_nodes)%num_outputs = 0
    end if
    
    svr_idx = find_node_index('svr')
    out_idx = find_node_index('out')
    dac_idx = find_node_index('dac')
    fft_idx = find_node_index('fft')

    
    print '(A)', "Day 11: Reactor Part 2"
    print '(A)', "======================"
    print '(A,I0,A)', "Graph loaded: ", num_nodes, " nodes"
    print '(A,I0)', "'svr' index: ", svr_idx
    print '(A,I0)', "'out' index: ", out_idx
    print '(A,I0)', "'dac' index: ", dac_idx
    print '(A,I0)', "'fft' index: ", fft_idx
    
    if (svr_idx == 0 .or. out_idx == 0 .or. dac_idx == 0 .or. fft_idx == 0) then
        print *, "ERROR: Could not find required nodes"
        stop
    end if
    
    memo_count = 0
    memo(:)%exists = .false.
    
    print *, ""
    print *, "Computing paths using DP..."    
    result = count_paths_dp(svr_idx, out_idx, "dac fft")
    
    print *, ""
    print *, "========================================"
    print '(A,I0)', "Total Path Count: ", result
    print *, "========================================"
    
contains

    subroutine parse_input(filename)
        character(len=*), intent(in) :: filename
        integer :: unit, ios
        character(len=100) :: line
        character(len=10) :: node_name
        character(len=100) :: outputs_str
        integer :: colon_pos
        
        unit = 10
        open(unit=unit, file=filename, status='old', action='read', iostat=ios)
        
        if (ios /= 0) then
            print *, "Error opening file"
            stop
        end if
        
        num_nodes = 0
        
        do
            read(unit, '(A)', iostat=ios) line
            if (ios /= 0) exit
            
            if (len_trim(line) == 0) cycle
            
            colon_pos = index(line, ':')
            if (colon_pos == 0) cycle
            
            num_nodes = num_nodes + 1
            if (num_nodes > MAX_NODES - 1) then
                print *, "Error: Too many nodes"
                stop
            end if
            
            node_name = adjustl(line(1:colon_pos-1))
            graph(num_nodes)%name = node_name
            
            outputs_str = line(colon_pos+1:)
            call parse_outputs(outputs_str, graph(num_nodes)%outputs, &
                              graph(num_nodes)%num_outputs)
        end do
        
        close(unit)
        
    end subroutine parse_input
    
    subroutine parse_outputs(str, outputs, num_outputs)
        character(len=*), intent(in) :: str
        character(len=10), intent(out) :: outputs(:)
        integer, intent(out) :: num_outputs
        integer :: i, len_str, start_pos
        
        num_outputs = 0
        start_pos = 1
        len_str = len_trim(str)
        
        i = 1
        do while (i <= len_str)
            do while (i <= len_str .and. str(i:i) == ' ')
                i = i + 1
            end do
            
            if (i > len_str) exit
            
            start_pos = i
            do while (i <= len_str .and. str(i:i) /= ' ')
                i = i + 1
            end do
            
            num_outputs = num_outputs + 1
            if (num_outputs > MAX_OUTPUTS) exit
            outputs(num_outputs) = adjustl(str(start_pos:i-1))
        end do
        
    end subroutine parse_outputs
    
    function find_node_index(name) result(idx)
        character(len=*), intent(in) :: name
        integer :: idx
        integer :: i
        
        idx = 0
        do i = 1, num_nodes
            if (trim(graph(i)%name) == trim(name)) then
                idx = i
                return
            end if
        end do
    end function find_node_index
    
    function encode_remaining(remaining_nodes) result(encoded)
        character(len=*), intent(in) :: remaining_nodes
        character(len=100) :: encoded
        ! For simplicity, just return the remaining nodes string
        encoded = remaining_nodes
    end function encode_remaining
    
    function make_memo_key(node_idx, remaining) result(key)
        integer, intent(in) :: node_idx
        character(len=*), intent(in) :: remaining
        character(len=100) :: key
        character(len=10) :: node_str
        
        write(node_str, '(I10)') node_idx
        key = trim(adjustl(node_str)) // "||" // trim(remaining)
    end function make_memo_key
    
    function lookup_memo(key) result(val)
        character(len=*), intent(in) :: key
        integer(kind=8) :: val
        integer :: i
        
        val = -1
        do i = 1, memo_count
            if (trim(memo(i)%key) == trim(key) .and. memo(i)%exists) then
                val = memo(i)%value
                return
            end if
        end do
    end function lookup_memo
    
    subroutine store_memo(key, val)
        character(len=*), intent(in) :: key
        integer(kind=8), intent(in) :: val
        integer :: i
        
        do i = 1, memo_count
            if (trim(memo(i)%key) == trim(key)) then
                memo(i)%value = val
                memo(i)%exists = .true.
                return
            end if
        end do
        
        if (memo_count < MAX_MEMO) then
            memo_count = memo_count + 1
            memo(memo_count)%key = key
            memo(memo_count)%value = val
            memo(memo_count)%exists = .true.
        else
            print *, "ERROR: Memo table full!"
        end if
    end subroutine store_memo
    
    recursive function count_paths_dp(current, end_node, remaining) result(count)
        integer, intent(in) :: current, end_node
        character(len=*), intent(in) :: remaining
        integer(kind=8) :: count
        integer :: i, next_idx
        integer(kind=8) :: memo_val
        character(len=100) :: memo_key, new_remaining
        character(len=10) :: neighbor
        
        if (current == end_node) then
            if (len_trim(remaining) == 0) then
                count = 1
            else
                count = 0
            end if
            return
        end if
        
        memo_key = make_memo_key(current, remaining)
        memo_val = lookup_memo(memo_key)
        
        if (memo_val >= 0) then
            count = memo_val
            return
        end if
        
        if (graph(current)%num_outputs == 0) then
            call store_memo(memo_key, int(0, kind=8))
            count = 0
            return
        end if
        
        count = 0
        
        do i = 1, graph(current)%num_outputs
            neighbor = graph(current)%outputs(i)
            next_idx = find_node_index(neighbor)
            
            if (next_idx > 0) then
                new_remaining = remove_from_remaining(remaining, neighbor)
                
                count = count + count_paths_dp(next_idx, end_node, new_remaining)
            end if
        end do
        
        call store_memo(memo_key, count)
        
    end function count_paths_dp
    
    function remove_from_remaining(remaining, node) result(new_remaining)
        character(len=*), intent(in) :: remaining, node
        character(len=100) :: new_remaining
        integer :: pos, len_remaining, len_node
        
        new_remaining = remaining
        
        pos = index(new_remaining, trim(node))
        
        if (pos > 0) then
            len_remaining = len_trim(new_remaining)
            len_node = len_trim(node)
            
            if (pos == 1) then
                if (len_remaining > len_node) then
                    new_remaining = adjustl(new_remaining(len_node+2:))
                else
                    new_remaining = ""
                end if
            else
                new_remaining = trim(new_remaining(1:pos-1)) // " " // &
                               adjustl(new_remaining(pos+len_node:))
                new_remaining = adjustl(new_remaining)
            end if
        end if
        
    end function remove_from_remaining

end program day11_p2
