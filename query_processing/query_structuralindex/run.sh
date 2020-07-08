./query_structuralindex -i ../../data/robots.edge -k 2 -o test_query -if ../../constructing_index/construct_structuralindex/index/robots_2 -qw ../../workload/query_fulllabel4k2
./query_structuralindex -i ../../data/robots.edge -k 2 -o test_query_wa -if ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workload -qw ../../workload/query_fulllabel4k2 -iw ../../workload/query_fulllabel4k2

./query_structuralindex -i ../../data/robots.edge -k 2 -o test_mg -mg ./maintenance_graph -if ../../constructing_index/construct_structuralindex/index/robots_2  -qw ../../workload/query_fulllabel4k2
./query_structuralindex -i ../../data/robots.edge -k 2 -o test_wa_mg -mg ./maintenance_graph -if ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workload -iw ../../workload/query_fulllabel4k2  -qw ../../workload/query_fulllabel4k2
./query_structuralindex -i ../../data/robots.edge -k 2 -o test_mw -mw ./maintenance_workload -if ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workload -iw ../../workload/query_fulllabel4k2  -qw ../../workload/query_fulllabel4k2

./query_structuralindex -i ../../data/robots.edge -k 2 -o test_query_mg -if ../../constructing_index/construct_structuralindex/index/robots_2MG -qw ../../workload/query_fulllabel4k2
./query_structuralindex -i ../../data/robots.edge -k 2 -o test_query_wa_mg -if ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workloadMG -qw ../../workload/query_fulllabel4k2 -iw ../../workload/query_fulllabel4k2
./query_structuralindex -i ../../data/robots.edge -k 2 -o test_query_mw -if ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workloadMW -qw ../../workload/query_fulllabel4k2 -iw ../../constructing_index/construct_wastructuralindex/index/robots_2_wa_workloadMW_workload
