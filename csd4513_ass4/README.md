arxeia:
-include/linux/sched.h
-kernel/set_task_params.c
-kernel/get_task_params.c
-kernel/sched.c

gia ola ta tests:
-header: task_params.h 

gia to group fairness gia ta processes ta opoia kaloun thn set_task_params gia to arxeio sched.c:
eftiaksa mia kainourgia synarthsh thn number_of_processes_in_group 
h opoia metraei posa processes einai se kathe group,
sto struct rq prosthesa ena kainourgio pedio to number_of_groups
sth synexeia prosthesa sthn context_switch ena kommati kwdika to opoio
ypologizei to xrono ekteleshs kathe process analoga me to group pou anhkei
kai eggyatai thn dikaih dianomh ths CPU se kathe group
eftiaksa mia kainourgia pick next task thn pick_next_gf_task xrisimopoiontas
ta scheduling entities h opoia ypologizei to neo 
execution runtime analoga me ton arithmo ton processes pou einai sto group kai ananeonei to athroisma
synolikou xronou gia to sygkekrimeno task.
telos allaksa th schedule anti na kalei thn pick_next_task na kalei thn kainourgia pick_next_gf_task

sto arxeio sched.h:
sto task struct prosthesa tria pedia ta group_name, member_id kai to group_runtime
sto sched_entity prosthesa to group_name kai to group_task_count

epishs tropopoihsa ligo ta get_task_params.c kai set_task_params.c gia na doyleyoyn pio swsta opws kai diorthosa kai kapoia arxeia pou eprepe na tropoppoihsw sthn ask3 dioti den doyleye swsta gi ayto tha ta ksanasteilw

parolaayta to qemu crasharei kai otan prospathisa na to trekso ektos qemu mou ebgaze ayto to error

fatal error: linux/compiler-gcc10.h: No such file or directory
   90 | #include gcc_header(__GNUC__)
      | ^~~~~
compilation terminated.
make[1]: *** [/spare/csd4513/linux-2.6.38.1/./Kbuild:36: kernel/bounds.s] Error 1
make: *** [Makefile:955: prepare0] Error 2
