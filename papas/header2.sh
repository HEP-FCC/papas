#for filename in  Path ParticlePData
#do
   find . -type f -name "*.h" -exec sed -i sed -i -e 's/\"Path.h\"/\"datatypes\/papas\/Path.h\"/g' {} +
#done