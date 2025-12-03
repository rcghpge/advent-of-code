program day3_p2_joltage_tester
  implicit none
  character(len=100)::line
  integer::n,i,k
  integer(kind=8)::total,bank_max
  integer::digits(100)
  character(len=12)::dp(100,1:12)
  character(len=13)::temp_str
  
  total=0
  
  do
    read(*,'(A)',end=10) line
    n=len_trim(line)
    
    if(n<=12)then
      bank_max=0
      do i=1,n
        bank_max=bank_max*10+(iachar(line(i:i))-iachar('0'))
      end do
    else
      do i=1,n
        do k=1,12
          dp(i,k)=''
        end do
      end do
      
      do i=1,n
        dp(i,1)=line(i:i)
      end do
      
      do i=2,n
        do k=1,min(i,12)
          if(len_trim(dp(i-1,k))>0)then
            if(len_trim(dp(i,k))==0 .or. dp(i-1,k)>dp(i,k))then
              dp(i,k)=dp(i-1,k)
            end if
          end if
          
          if(k>=2 .and. len_trim(dp(i-1,k-1))>0)then
            temp_str=trim(dp(i-1,k-1))//line(i:i)
            if(len_trim(dp(i,k))==0 .or. temp_str>dp(i,k))then
              dp(i,k)=temp_str(1:12)
            end if
          end if
        end do
      end do
      
      bank_max=0
      if(len_trim(dp(n,12))>0)then
        do i=1,12
          bank_max=bank_max*10+(iachar(dp(n,12)(i:i))-iachar('0'))
        end do
      end if
    end if
    
    total=total+bank_max
  end do
  
10 print '(A,I0)', 'Total output joltage: ', total
end program day3_p2_joltage_tester
