<MODULE class=PPM name=GradientPPM
        tip="<p>Convert PPM to abs-grad and angle-grad</p>"
	cmd="ppmgrad @input@ @abs@ @angle@">
  <INPUT name=input label="PPM File: " tip="PPM Input File">
  <OUTPUT name=abs depend=input label="Abs: " tip="Absolute gradient">
  <OUTPUT name=angle depend=input label="Angle: " tip="Angle gradient">
</MODULE>

