echo creating hex engine with name ${1}HexEngine
for fil in SimpleHexEngine.h SimpleHexEngine.cxx; do
    cat $fil | sed "s/SimpleNAMESPACE/${1}NAMESPACE/g" |  sed "s/SimpleHexEngine/${1}HexEngine/ig" > ${1}HexEngine`echo $fil | sed "s/SimpleHexEngine//g"`
done