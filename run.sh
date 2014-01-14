
path='/ifs/scratch/c2b2/ip_lab/rs2740/20120424/WF_1K_'
n=4000
#file=gen.1000.$n.1.WF_1K_$n.trees.match
match_file=WF_1K.$n.ARRAY.gline.match
ped_file=gen.1000.$n.1.WF_1K_$n.SEQ.ped

./main $path$n/$ped_file $path$n/$match_file > $n.out.ped 

