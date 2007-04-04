<MODULE class=GaImage name=convert2ndvi
        tip="<p>NDVI Image from IRRG Image</p>"
	cmd="gi_ndviimage @input@ @output@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <OUTPUT name=output depend=input label="Out: " tip="NDVI Image from IRRG Image">
</MODULE>

