<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
	<rtf>
		<xsl:apply-templates select="//shpinst"/>
	</rtf>
</xsl:template>
<xsl:template match="//shpinst[shptxt]">
	<text>
		<xsl:attribute name="left"><xsl:value-of select="shpleft/@arg" /></xsl:attribute>
		<xsl:attribute name="top"><xsl:value-of select="shptop/@arg" /></xsl:attribute>
		<xsl:attribute name="right"><xsl:value-of select="shpright/@arg" /></xsl:attribute>
		<xsl:attribute name="bottom"><xsl:value-of select="shpbottom/@arg" /></xsl:attribute>
		<xsl:value-of select="normalize-space(shptxt/f/text())" />
	</text>
</xsl:template>
<xsl:template match="*">
</xsl:template>
</xsl:stylesheet>
