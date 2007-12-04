<MODULE class=GaImage name=Plantation
        tip="<p>Find plantations in sattelite images</p>"
	cmd="gi_plantation @input@ @output@ @maxClusterEdge@ @maxMeanDeviation@">
	<INPUT name=input label="Input Image: ">
	<OUTPUT name=output label="Output Labelimage: ">
	<TEXT name=maxClusterEdge label="Maximum Cluster Edge: " cmd="@maxClusterEdge@" value="50.0">
	<TEXT name=maxMeanDeviation label="Maximum Mean Deviation: " cmd="@maxMeanDeviation@" value="2.0">
</MODULE>
