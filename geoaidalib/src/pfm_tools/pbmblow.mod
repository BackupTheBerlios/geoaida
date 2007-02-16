<MODULE class=Bitmap name=pbmblow
        tip="<p>Blow a bitmap</p>"
	cmd="pbmblow @input@ @output@ @fg_color@ @no_loops@">
  <INPUT name=input label="PLM File: " tip="PLM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
  <TEXT name=fg_color cmd="@fg_color@" value=1>
  <TEXT name=no_loops value=1>
</MODULE>

