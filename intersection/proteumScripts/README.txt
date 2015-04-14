Steps to execute scripts:

To set my directory in:
cd /vagrant_box/testAndre/intersection/proteumScripts

1) Creating the test session with whole operators:
./scripts/CreatingTestSession.pl /home/andrelobo/vagrant_box/mtrepair/intersection/proteumScripts/research research three-step three-step-andre

2) Importing and executing whole test cases:
i) to set a positive execution
./scripts/ImportingExecutingTestSet.pl /home/andrelobo/vagrant_box/mtrepair/intersection/proteumScripts/research pool poke 1 5 three-step three-step-andre

ii) to set a negative execution
./scripts/ImportingExecutingTestSet.pl /home/andrelobo/vagrant_box/mtrepair/intersection/proteumScripts/research pool poke 1 1 three-step three-step-andre

---------------------------------



Others Informations:

3) Gerando relatório de cobertura para cada um dos 801 casos de teste
CoverageByTestCase.pl /home/auri/temp/proteum/research/  /home/auri/temp/proteum/scripts pool 1 801 comm comm-im

4) Gerando relatório de cobertura para o conjunto de teste completo
CoverageByTestSet.pl /home/auri/temp/proteum/research/  /home/auri/temp/proteum/scripts pool 1 801 comm comm-im

5) Gerando escore de mutação e mutantes vivos e mortos para cada um dos 801 casos de teste
DeadLiveMutantsByTestCase.pl /home/auri/temp/proteum/research/ /home/auri/temp/proteum/scripts 1 801 comm comm-im

6) Gerar mapa de bits para detectar quais mutantes foram mortos por quais casos de teste
java -cp /home/auri/temp/proteum/scripts/TestCaseMutant-1.0.jar br.ufg.inf.tcm.TestCaseMutant 1 801 1869 /home/auri/temp/proteum/research/comm/result comm /home/auri/temp/proteum/research/comm/comm-im/

7) Quando necessário guardar os números dos equivalentes
muta -l <SESSAO-TESTE> | equivs > comm-equivs

Configuracao variavel de ambiente:
source .profile
gedit .profile
export PROTEUMIMHOME=/home/andrelobo/Dropbox/proteum/tool/proteumIM2.0-3k/LINUX/bin
export PATH=$PROTEUMIMHOME:$PATH
