
install4cbfromvs()
{

project_vs="$1.vcxproj"
project_cbp="$1.cbp"
project_cbp_temp="$1"
project_cbp_temp_file=""
cbp_begin=0
cbp_end=0
#"" ".cbp.temp"
#cat NFComm/NFCore/NFCore.cbp | awk 'BEGIN{ print "Start" } {for(i=0;i<NF;i++) {printf $0; printf "\n"  }} END{print cbp_begin}'
#cat NFComm/NFCore/NFCore.cbp | awk 'BEGIN{} {cbp_begin=NR; printf $1; printf "\n" } END{print cbp_begin}'
cat $project_cbp | awk 'BEGIN{}
{
    project_cbp_temp_file= "'$project_cbp_temp'" ""  ".cbp.temp";

    if($1=="<Unit") 
    {
        if (cbp_begin<=0)
        {
            cbp_begin=NR;
        }
    }
    else
    {
        if (cbp_begin>0 && cbp_end<=0)
        {
            cbp_end=NR; 
        }
    }

    if (cbp_begin<=0)
    {
        print $0 > project_cbp_temp_file
    }
} 
END{
}'

#var_line=“<Unit filename=” + substr($0, 19, length($0) - 19);
#cat NFComm/NFCore/NFCore.vcxproj | awk 'BEGIN{} { print $0 >> "1.log"} END{}'
cat $project_vs | awk 'BEGIN{} 
{ 
    project_cbp_temp_file= "'$project_cbp_temp'" ""  ".cbp.temp";
    if($1=="<ClInclude" || $1=="<ClCompile") 
    {
        info=$0;
        start=index(info, "=");
        _end=index(info, ">");
		
		
        if (index(info, "/>") > 0)
        {
			data="<Unit filename" "" substr(info, start, length($0));
        }
        else
        {
            data="<Unit filename" "" substr(info, start, _end - start) "" "/>";
        }
        
        

        print data >> project_cbp_temp_file
    }
} END{}'

cat $project_cbp | awk 'BEGIN{}
{
    project_cbp_temp_file= "'$project_cbp_temp'" ""  ".cbp.temp";
    if($1=="<Unit") 
    {
        if (cbp_begin<=0)
        {
            cbp_begin=NR;
        }
    }
    else
    {
        if (cbp_begin>0 && cbp_end<=0)
        {
            cbp_end=NR; 
        }
    }

    if (cbp_end>0 && NR>=cbp_end)
    {
        print $0 >> project_cbp_temp_file
    }
} 
END{
}'

project_cbp_old="$1.cbp.old"
project_cbp_temp_file="$1.cbp.temp"

rm -f $project_cbp_old
mv $project_cbp $project_cbp_old
mv $project_cbp_temp_file $project_cbp

echo "Converted file: $project_vs to $project_cbp !!"

}


array_name=(
NFTools/NFFileProcess/NFFileProcess
NFComm/NFCore/NFCore
NFComm/NFMessageDefine/NFMessageDefine
NFComm/NFNetPlugin/NFNetPlugin
NFComm/NFKernelPlugin/NFKernelPlugin
NFComm/NFConfigPlugin/NFConfigPlugin
NFComm/NFLogPlugin/NFLogPlugin
NFComm/NFPluginLoader/NFPluginLoader
NFComm/NFActorPlugin/NFActorPlugin
NFComm/NFMysqlPlugin/NFMysqlPlugin
NFComm/NFNoSqlPlugin/NFNoSqlPlugin
NFComm/NFUrlPlugin/NFUrlPlugin
NFServer/NFGameServerNet_ClientPlugin/NFGameServerNet_ClientPlugin
NFServer/NFGameServerNet_ServerPlugin/NFGameServerNet_ServerPlugin
NFServer/NFGameLogicPlugin/NFGameLogicPlugin
NFServer/NFGameServerPlugin/NFGameServerPlugin
NFServer/NFDataAgent_NosqlPlugin/NFDataAgent_NosqlPlugin
NFServer/NFLoginNet_ClientPlugin/NFLoginNet_ClientPlugin
NFServer/NFLoginNet_ServerPlugin/NFLoginNet_ServerPlugin
NFServer/NFLoginLogicPlugin/NFLoginLogicPlugin
NFServer/NFMasterNet_ServerPlugin/NFMasterNet_ServerPlugin
NFServer/NFMasterServerPlugin/NFMasterServerPlugin
NFServer/NFProxyLogicPlugin/NFProxyLogicPlugin
NFServer/NFProxyServerNet_ClientPlugin/NFProxyServerNet_ClientPlugin
NFServer/NFProxyServerNet_ServerPlugin/NFProxyServerNet_ServerPlugin
NFServer/NFWorldLogicPlugin/NFWorldLogicPlugin
NFServer/NFWorldNet_ClientPlugin/NFWorldNet_ClientPlugin
NFServer/NFWorldNet_ServerPlugin/NFWorldNet_ServerPlugin
)


array_index=0
array_step=1
for data in ${array_name[@]}  
do
    #echo ${array_name[$array_index]}

    install4cbfromvs ${array_name[$array_index]}

    array_index=`expr $array_index + $array_step`
    
done 


echo. & pause 