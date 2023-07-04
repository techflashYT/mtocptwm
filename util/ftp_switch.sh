ftp << EOF
open $1 $2
anon
pass
cd switch
put bin/mtocptwm.nro mtocptwm.nro
quit
EOF