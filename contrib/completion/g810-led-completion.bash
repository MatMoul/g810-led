#/usr/bin/env bash

# OPTIONS
# #######
g810_device_options="-dv -dp -ds -tuk"
g810_tuk_option="-tuk"
g810_tuk_options="1 2 3"
g810_options="$g810_device_options g810_tuk_option"

# COMMANDS
# ########

# effects
effects_command="-fx"
effects="color breathing cycle waves hwave vwave cwave"
effects_target="all keys logo"

# startup mode
startup_mode_command="--startup-mode"
startup_modes="wave color"

# keygroup commands
key_group_commands="-g -gn"
key_groups="logo indicators fkeys gkeys modifiers multimedia arrows numeric functions keys"

# g810 commands and options
simple_long_commands="--list-keyboards --print-device --help --help-keys --help-effects --help-samples"

#   ALL
# ########
all="-a -k -an -kn -c $startup_mode_command $simple_long_commands $g810_options $key_group_commands"



_g810-led_completions()
{
    # ignore already typed in g810 options
    # TODO check for parameter, and avoid duplicated options
    # ######################################################
    local i=1
    while [[ $g810_options =~ ${COMP_WORDS[i]} &&  ${COMP_CWORD} -gt $i ]]
    do
	((i++))
    done
    
    # check for simple long commands, that dont have parameters
    # #########################################################
    local contains=0
    local exactmatch=0
    local opt
    for opt in $simple_long_commands
    do
	if [[ $opt =~ "${COMP_WORDS[i]}" ]]; then let contains=contains+1;     fi
	if [[ $opt == "${COMP_WORDS[i]}" ]]; then let exactmatch=exactmatch+1; fi
    done
    if [[ $contains -eq 1 && $exactmatch -eq 1 ]]; then return; fi;

    # long commands with parameter
    # ############################

    # --> startup mode
    if [[ $startup_mode_command == "${COMP_WORDS[i]}" ]]
    then
	((i++))
	if [[ " $startup_modes " =~ " ${COMP_WORDS[i]} " ]]
	then
	    return;
	else
	    COMPREPLY=($(compgen -W "$startup_modes" --  ${COMP_WORDS[i]}))
	    return
	fi
    fi

    # --> effects
    if [[ $effects_command == "${COMP_WORDS[i]}" ]]
    then
	((i++))
	if [[ " $effects " =~ " ${COMP_WORDS[i]} " ]]
	then
	    ((i++))
	    if [[ " $effects_target " =~  " ${COMP_WORDS[i]} " ]]
	    then
		return
	    else
		COMPREPLY=($(compgen -W "$string" -- "${COMP_WORDS[i]}" ))
		return
	    fi
	else
	    COMPREPLY=($(compgen -W "$effects" -- "${COMP_WORDS[i]}" ))
	    return
	fi	
    fi

    # commands with keygroups
    # #######################
    if [[ " $key_group_commands " =~ " ${COMP_WORDS[i]} " ]]
    then
	((i++))
	if [[ " $key_groups " =~ " ${COMP_WORDS[i]} " ]]
	then
	    return
	else
	    COMPREPLY=($(compgen -W "$key_groups" --  ${COMP_WORDS[i]}))
	    return
	fi
    fi
    
    
    # complete
    # ########
    COMPREPLY=($(compgen -W "$all" -- ${COMP_WORDS[i]}))
    return
}  

complete -F _g810-led_completions g810-led
