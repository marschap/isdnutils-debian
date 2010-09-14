#!_WISH_ -f

# You can configure the actions to be performed when a channel
# changes it's state. Each action is simply execution of an
# external command. The online-action-strings can contain a '%s'
# which will be replaced by the remote Phonenumber.
# The following definitions are just Samples
#set online_action_in  {rsh wuemaus "splay -q /sounds/ring &"}
#set online_action_out {rsh wuemaus "dialsound %s &"}
#set offline_action    {rsh wuemaus "splay -q /sounds/hangup &"}

# Uncomment these to disable actions.
set online_action_out ""
set online_action_in  ""
set offline_action    ""

set infodev /dev/isdninfo
set infoexit 0

set usage {None Raw Modem Net Voice Fax}

proc getinfo {} {
  global infodev is
  
  set f [open $infodev r]
  gets $f is(idmap)
  gets $f is(chmap)
  gets $f is(drmap)
  gets $f is(usage)
  gets $f is(flags)
  gets $f is(phone)
  close $f
}

proc signal_online {out num} {
  global online_action_out online_action_in

  if {$out} {
    if {![string length $online_action_out]} {
      return
    }
    if {[string first %s $online_action_out]!=-1} {
      catch {eval exec [format $online_action_out $num] < /dev/null &}
    } else {
      catch {eval exec $online_action_out < /dev/null &}
    }
  } else {
    if {![string length $online_action_in]} {
      return
    }
    if {[string first %s $online_action_in]!=-1} {
      catch {eval exec [format $online_action_in $num] < /dev/null &}
    } else {
      catch {eval exec $online_action_in < /dev/null &}
    }
  }
}

proc signal_offline {} {
  global offline_action

  if {![string length $offline_action]} {
    return
  }
  catch {eval exec $offline_action < /dev/null &}
}

proc doloop {} {
  global infoexit is usage yesno onl

  if {$infoexit} {
    exit
  }
  catch {getinfo}
  set flist [lreplace $is(flags) 0 0]
  set ulist [lreplace $is(usage) 0 0]
  set ilist [lreplace $is(chmap) 0 0]
  set rlist [lreplace $is(phone) 0 0]
  set dcount 0
  foreach n $flist {
    if [string compare $n ?] {
      incr dcount
    }
  }
  for {set i 0} {$i<$dcount} {incr i} {
    set chcount 0
    set chi 0
    set clist [lreplace $is(drmap) 0 0]
    foreach n $clist {
      if {![string compare $n $i]} {
	set uplain [expr [lindex $ulist $chi]&7]
	set outgoing [expr [lindex $ulist $chi]&128]
	set usg($i,$chcount) [lindex $usage $uplain]
	set rem($i,$chcount) [lindex $rlist $chi]
	if [expr [lindex $flist $i] & (1<<$chcount)] {
	  if {![string compare $onl($i,$chcount) No]} {
	    signal_online $outgoing $rem($i,$chcount)
	  }
	  set onl($i,$chcount) "Yes"
	  if {$outgoing} {
	    set onlc($i,$chcount) red
          } else {
	    set onlc($i,$chcount) yellow
          }
	} else {
	  if {![string compare $onl($i,$chcount) Yes]} {
	    signal_offline
	  }
	  set onl($i,$chcount) "No"
	  set onlc($i,$chcount) green
	}
	incr chcount
      }
      incr chi
    }
    for {set j 0} {$j<$chcount} {incr j} {
      .did$i.st$j.onl configure -text $onl($i,$j) -background $onlc($i,$j)
      .did$i.st$j.usg configure -text $usg($i,$j)
      .did$i.st$j.rem configure -text $rem($i,$j)
    }
  }
  after 100 doloop
}

catch {getinfo}
if [info exists is(flags)] {
  set flist [lreplace $is(flags) 0 0]
  set ulist [lreplace $is(usage) 0 0]
  set rlist [lreplace $is(phone) 0 0]
  set dcount 0
  foreach n $flist {
    if [string compare $n ?] {
      incr dcount
    }
  }
  message .msg -width 3i -text "No ISDN-drivers loaded" \
    -font -Adobe-Times-Medium-R-Normal-*-180-*
  if {$dcount==0} {
    pack .msg
  } else {
    for {set i 0} {$i<$dcount} {incr i} {
      set chcount 0
      set clist [lreplace $is(drmap) 0 0]
      foreach n $clist {
	if {![string compare $n $i]} {
	  set did [lindex $is(idmap) $i]
	  set uplain [expr [lindex $ulist $i]&7]
	  set outgoing [expr [lindex $ulist $i]&128]
	  set usg($i,$chcount) [lindex $usage $uplain]
	  if [expr [lindex $flist $i] & (1<<$chcount)] {
	    set onl($i,$chcount) "Yes"
	    if {$outgoing} {
	      set onlc($i,$chcount) red
	    } else {
	      set onlc($i,$chcount) yellow
	    }
	  } else {
	    set onl($i,$chcount) "No"
	    set onlc($i,$chcount) green
	  }
	  set rem($i,$chcount) [lindex $rlist $i]
	  incr chcount
	}
      }
      frame .did$i -relief ridge -borderwidth 2
      label .did$i.id -text "DriverID: $did"
      pack .did$i.id
      for {set j 0} {$j<$chcount} {incr j} {
	frame .did$i.st$j
	label .did$i.st$j.l1 -text "Channel: $j" -width 10 -anchor w
	label .did$i.st$j.l2 -text "Online:" -width 6 -anchor w
	label .did$i.st$j.onl -width 4 -anchor w -relief ridge \
	  -text $onl($i,$j) -background $onlc($i,$j)
	label .did$i.st$j.l3 -text "  Usage:" -width 7 -anchor w
	label .did$i.st$j.l4 -text "  Remote-#:" -anchor w
	label .did$i.st$j.rem -text $rem($i,$j) -width 20 -anchor w \
	  -relief ridge
	label .did$i.st$j.usg -width 6 -anchor w -relief ridge \
	  -text $usg($i,$j)
	pack .did$i.st$j.l1 .did$i.st$j.l2 .did$i.st$j.onl .did$i.st$j.l4 \
	  .did$i.st$j.rem -side left
	pack .did$i.st$j.usg .did$i.st$j.l3 -side right
	pack .did$i.st$j -side top -padx 3 -pady 3 -fill x
      }
      pack .did$i -fill x -padx 3 -pady 3
    }
  }
  frame .but
  button .but.quit -text Quit -command "set infoexit 1"
  pack .but.quit -fill x
  pack .but -fill x
  set f [open |hostname r]
  wm title . "ISDN-Status of [gets $f]"
  close $f
  doloop
} else {
  wm withdraw .
  tk_dialog .err Error "No ISDN-infodevice found" error 0 "Exit"
  exit
}








