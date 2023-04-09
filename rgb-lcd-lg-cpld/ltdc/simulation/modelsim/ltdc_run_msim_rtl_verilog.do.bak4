transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+/home/jlywxy/project/ltdc {/home/jlywxy/project/ltdc/ltdc.v}

vlog -vlog01compat -work work +incdir+/home/jlywxy/project/ltdc/simulation/modelsim {/home/jlywxy/project/ltdc/simulation/modelsim/ltdc.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L maxii_ver -L rtl_work -L work -voptargs="+acc"  ltdc_vlg_tst

add wave *
view structure
view signals
run -all
