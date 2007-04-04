<MODULE class=Misc name="Connect Data" cmd="ln -sf @input@ @output@">
  <INPUT name=input>
  <OUTPUT name=output depend=input>
</MODULE>
<MODULE class=Misc name="External Command"
        tip="<p>Executes an arbitrary external operator!</p>"
	cmd="@operator@ @options@ @input@ @output@ ">
  <INPUT name=input label="Source File: " tip="Input File">
  <OUTPUT name=output depend=input label="Destination File: " tip="Output File">
  <TEXT name=operator label="command:" tip="<p>Name of external operator</p>">
  <TEXT name=options label="options:" tip="<p>Options to pass to external operator</p>"">
</MODULE>
