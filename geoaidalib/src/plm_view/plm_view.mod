<MODULE class=Display name=plmView
        tip="Show Region-Files"
	cmd="plm_view @regionfile@ @labelfile@ @datafile@">
  <INPUT name=regionfile label="Regionfile" cmd="-r @regionfile@" optional>
  <INPUT name=labelfile label="PLM File" cmd="-l @labelfile@" >
  <INPUT name=datafile label="Dataimage" cmd="-d @datafile@" optional >
</MODULE>
