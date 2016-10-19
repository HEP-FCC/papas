
find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"PParticle.h\"/\"datatypes\/papas\/Particle.h\"/g"
find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"PParticle.h\"/\"datatypes\/papas\/Particle.h\"/g"
find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"pTrack.h\"/\"datatypes\/papas\/Track.h\"/g"
find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"pTrack.h\"/\"datatypes\/papas\/Track.h\"/g"

for filename in  Path ParticlePData Cluster Definitions DefinitionsCollections Helix Id ParticlePData Path SimParticle
do
	for dirname in datatypes
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done

for filename in  Calorimeter CMS CMSEcal CMSField CMSHcal CMSTracker Detector DetectorElement Field Material SurfaceCylinder Tracker VolumeCylinder
do
	for dirname in detectors
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done

for filename in  AliceDisplay Display Drawable GBlob GDetector GDetectorElement GTrajectories GTrajectory ViewPane
do
	for dirname in display
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done

for filename in  DefinitionsNodes DirectedAcyclicGraph Distance Edge EventRuler FloodFill GraphBuilder Ruler
do
	for dirname in graphtools
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done


for filename in  BlockBuilder BlockSplitter MergedClusterBuilder PapasManager PFBlock PFBlockBuilder PFEvent PFEventDisplay PFReconstructor
do
	for dirname in reconstruction
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done

for filename in  HelixPropagator Propagator StraightLinePropagator Simulator
do
	for dirname in simulation
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done


for filename in  DeltaR GeoTools Log PDebug PrettyPrinter Random StringFormatter
do
	for dirname in utility
		do
			echo $filename
		  echo `grep -rl "\"$filename.h\"" *`  
		   find . -type f -name "*.h" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		   find . -type f -name "*.cpp" -print0 | xargs -0  sed -i.bu "s/\"${filename}.h\"/\"papas$\/${dirname}\/${filename}.h\"/g"
		done
done
