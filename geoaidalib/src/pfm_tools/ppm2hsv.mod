<MODULE class=PPM name=ppm2hsv
        tip="<p>Convert PPM to HSV</p><p>PFM - Portable Float Map</p>"
	cmd="ppm2hsv @input@ @h@ @s@ @v@">
  <INPUT name=input label="PFM File: " tip="PFM Input File">
  <OUTPUT name=h depend=input label="H File: " tip="H-PFM Output File">
  <OUTPUT name=s depend=input label="S File: " tip="S-PFM Output File">
  <OUTPUT name=v depend=input label="v File: " tip="V-PFM Output File">
</MODULE>

