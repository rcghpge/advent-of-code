program day11
    implicit none

    integer, parameter :: MAX_NODES = 1000
    integer, parameter :: MAX_NEIGHBORS = 30
    character(len=20) :: nodes(MAX_NODES)
    character(len=20) :: adj_list(MAX_NODES, MAX_NEIGHBORS)
    integer :: neighbor_count(MAX_NODES)
    integer :: num_nodes = 0
    integer :: path_count = 0
    logical :: visited(MAX_NODES)

    call build_graph(nodes, adj_list, neighbor_count, num_nodes)

    visited = .false.
    call dfs_count("you", "out", nodes, adj_list, neighbor_count, num_nodes, path_count, visited)

    print '(A,I0)', "Number of paths from you to out: ", path_count

contains

    subroutine build_graph(nodes, adj_list, neighbor_count, num_nodes)
        character(len=20), intent(inout) :: nodes(MAX_NODES)
        character(len=20), intent(inout) :: adj_list(MAX_NODES, MAX_NEIGHBORS)
        integer, intent(inout) :: neighbor_count(MAX_NODES)
        integer, intent(inout) :: num_nodes
        
        integer :: ios, i, source_idx, target_idx, neigh_idx
        character(len=256) :: line
        character(len=20) :: source, target
        integer :: colon_pos, pos

        num_nodes = 0
        neighbor_count = 0
        adj_list = "                    "

        do while (.true.)
            read(*, "(A)", iostat=ios) line
            if (ios /= 0) exit
            if (len_trim(line) == 0) cycle

            colon_pos = index(line, ":")
            if (colon_pos > 0) then
                source = trim(adjustl(line(:colon_pos-1)))
                source_idx = find_node(nodes, source, num_nodes)
                if (source_idx == 0) then
                    num_nodes = num_nodes + 1
                    nodes(num_nodes) = source
                    source_idx = num_nodes
                end if

                pos = colon_pos + 1
                do while (pos <= len_trim(line))
                    do while (pos <= len_trim(line) .and. line(pos:pos) == " ")
                        pos = pos + 1
                    end do
                    if (pos > len_trim(line)) exit

                    i = pos
                    do while (i <= len_trim(line) .and. line(i:i) /= " ")
                        i = i + 1
                    end do

                    target = trim(adjustl(line(pos:i-1)))
                    target_idx = find_node(nodes, target, num_nodes)
                    if (target_idx == 0) then
                        num_nodes = num_nodes + 1
                        nodes(num_nodes) = target
                        target_idx = num_nodes
                    end if

                    neigh_idx = neighbor_count(source_idx) + 1
                    if (neigh_idx <= MAX_NEIGHBORS) then
                        adj_list(source_idx, neigh_idx) = nodes(target_idx)
                        neighbor_count(source_idx) = neigh_idx
                    end if
                    pos = i + 1
                end do
            end if
        end do
    end subroutine build_graph

    recursive subroutine dfs_count(current, target, nodes, adj_list, &
                                  neighbor_count, num_nodes, count, visited)
        character(len=*), intent(in) :: current, target
        character(len=20), intent(in) :: nodes(MAX_NODES)
        character(len=20), intent(in) :: adj_list(MAX_NODES, MAX_NEIGHBORS)
        integer, intent(in) :: neighbor_count(MAX_NODES)
        integer, intent(in) :: num_nodes
        integer, intent(inout) :: count
        logical, intent(inout) :: visited(MAX_NODES)

        integer :: curr_idx, i
        character(len=20) :: neighbor

        if (trim(current) == trim(target)) then
            count = count + 1
            return
        end if

        curr_idx = find_node(nodes, current, num_nodes)
        if (curr_idx == 0 .or. curr_idx > num_nodes) return
        if (visited(curr_idx)) return

        visited(curr_idx) = .true.

        do i = 1, neighbor_count(curr_idx)
            neighbor = trim(adj_list(curr_idx, i))
            if (len_trim(neighbor) > 0) then
                call dfs_count(neighbor, target, nodes, adj_list, &
                              neighbor_count, num_nodes, count, visited)
            end if
        end do

        visited(curr_idx) = .false.
    end subroutine dfs_count

    function find_node(nodes, name, num_nodes) result(idx)
        character(len=20), intent(in) :: nodes(MAX_NODES)
        character(len=*), intent(in) :: name
        integer, intent(in) :: num_nodes
        integer :: idx, i

        idx = 0
        do i = 1, num_nodes
            if (trim(nodes(i)) == trim(name)) then
                idx = i
                exit
            end if
        end do
    end function find_node

end program day11
