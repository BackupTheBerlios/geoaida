<MODULE class=GaImage name=hough
        tip="<p>Generate Hough Map For Lines</p><p>PFM - Portable Float Map</p>"
	cmd="gi_hough -d @debug@ @level@ @input@ @output@ @histogram@ @dft_histogram@ @dft_values@ @anglemin@ @anglemax@ @anglestep@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <OUTPUT name=histogram depend=input label="angle histogram: " tip="PGM angle histogram file">
  <OUTPUT name=dft_histogram depend=input label="dft histogram: " tip="DFT histogram file">
  <OUTPUT name=dft_values depend=input label="dft values: " tip="DFT value file">
  <text name=anglemin label="Lower Angle (Degrees):">
  <text name=anglemax label="Upper Angle (Degrees):">
  <text name=anglestep label="Angle Step:">
  <OUTPUT name=debug label="Debug Image:">
  <TEXT name=level label="Debug Threshhold:"  value=100>
</MODULE>
