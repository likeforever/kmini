if sizeof(void *)==8
    set $64BITS = 1
else
    set $64BITS = 0
end

set print thread-events off
set confirm off
set breakpoint pending on
set follow-fork-mode child
set detach-on-fork off
set schedule-multiple on
set follow-exec-mode new
#set verbose off

set output-radix 0x10
set input-radix 0x10

# These make gdb never pause in its output
set height 0
set width 0

# Display instructions in Intel format
set disassembly-flavor intel




# __________hex/ascii dump an address_________
define ascii_char
    if $argc != 1
        help ascii_char
    else
        # thanks elaine :)
        set $_c = *(unsigned char *)($arg0)
        if ($_c < 0x20 || $_c > 0x7E)
            printf "."
        else
            printf "%c", $_c
        end
    end
end
document ascii_char
Print ASCII value of byte at address ADDR.
Print "." if the value is unprintable.
Usage: ascii_char ADDR
end


define hex_quad
    if $argc != 1
        help hex_quad
    else
        printf "%02X %02X %02X %02X %02X %02X %02X %02X", \
               *(unsigned char*)($arg0), *(unsigned char*)($arg0 + 1),     \
               *(unsigned char*)($arg0 + 2), *(unsigned char*)($arg0 + 3), \
               *(unsigned char*)($arg0 + 4), *(unsigned char*)($arg0 + 5), \
               *(unsigned char*)($arg0 + 6), *(unsigned char*)($arg0 + 7)
    end
end
document hex_quad
Print eight hexadecimal bytes starting at address ADDR.
Usage: hex_quad ADDR
end

define hexdump
    if $argc != 1
        help hexdump
    else
        echo \033[1m
        if ($64BITS == 1)
         printf "0x%016lX : ", $arg0
        else
         printf "0x%08X : ", $arg0
        end
        echo \033[0m
        hex_quad $arg0
        echo \033[1m
        printf " - "
        echo \033[0m
        hex_quad $arg0+8
        printf " "
        echo \033[1m
        ascii_char $arg0+0x0
        ascii_char $arg0+0x1
        ascii_char $arg0+0x2
        ascii_char $arg0+0x3
        ascii_char $arg0+0x4
        ascii_char $arg0+0x5
        ascii_char $arg0+0x6
        ascii_char $arg0+0x7
        ascii_char $arg0+0x8
        ascii_char $arg0+0x9
        ascii_char $arg0+0xA
        ascii_char $arg0+0xB
        ascii_char $arg0+0xC
        ascii_char $arg0+0xD
        ascii_char $arg0+0xE
        ascii_char $arg0+0xF
        echo \033[0m
        printf "\n"
    end
end
document hexdump
Display a 16-byte hex/ASCII dump of memory at address ADDR.
Usage: hexdump ADDR
end


# _______________data window__________________
define ddump
    if $argc != 1
        help ddump
    else
        echo \033[34m
        if ($64BITS == 1)
         printf "[0x%04X:0x%016lX]", $ds, $data_addr
        else
         printf "[0x%04X:0x%08X]", $ds, $data_addr
        end
	echo \033[34m
	printf "------------------------"
    printf "-------------------------------"
    if ($64BITS == 1)
     printf "-------------------------------------"
	end

	echo \033[1;34m
	printf "[data]\n"
        echo \033[0m
        set $_count = 0
        while ($_count < $arg0)
            set $_i = ($_count * 0x10)
            hexdump $data_addr+$_i
            set $_count++
        end
    end
end
document ddump
Display NUM lines of hexdump for address in $data_addr global variable.
Usage: ddump NUM
end


define dd
    if $argc == 0
        help dd
    end 

   if $argc == 1
        set $data_addr = $arg0
        ddump 0x10
    end

   if $argc == 2
        set $data_addr = $arg0
        ddump $arg1
   end 

end
document dd
Display 16 lines of a hex dump of address starting at ADDR.
Usage: dd ADDR
end


define bfi
   finish
   s
end

define grep
   set logging file gdb_like.txt
   set logging on 
   $arg0

   set logging off
end

#EOF
