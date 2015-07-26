## Introduction ##

Yellowcot introduces a standard, XML-based, open file format for multimedia flash card sets. The latest revision of the file format is 1.0.

## Revisions ##

### [Revision 1](https://code.google.com/p/yellowcot/source/detail?r=1).0 ###

Per [revision 1](https://code.google.com/p/yellowcot/source/detail?r=1).0, a Yellowcot file is an uncompressed tarball including an _index.xml_ and a _media_ directory. [Revision 1](https://code.google.com/p/yellowcot/source/detail?r=1).0 is identical to [revision 0](https://code.google.com/p/yellowcot/source/detail?r=0).4 except for the addition of a definition of the file format version in the _quiz_ tag. The _media_ directory stores audio, image, and video files numbered sequentially. The _index.xml_ file stores the questions and answers that make up the flash card set. An example _index.xml_ file is shown below. The markup defines two question and answer pairs.

```
<?xml version="1.0"?>
<quiz version="1.0">
	<qa>
		<q type="media">1</q>
		<qsrc>http://commons.wikimedia.org/wiki/File:Big-eared-townsend-fledermaus.jpg</qsrc>
		<qlic>PD</qlic>
		<a type="text">leðurblaka (fs)</a>
		<asrc></asrc>
		<alic></alic>
	</qa>
	<qa>
		<q type="media">2</q>
		<qsrc>http://commons.wikimedia.org/wiki/File:Female_pair.jpg</qsrc>
		<qlic>CC-BY-SA 2.0</qlic>
		<a type="text">hænsnakjöt (ns) / hænsni (ns) / kjúklingur (ms)</a>
		<asrc></asrc>
		<alic></alic>
	</qa>
</quiz>
```

Supported by Yellowcot versions: 1.2.4 - 1.2.6

Example ([revision 1](https://code.google.com/p/yellowcot/source/detail?r=1).0) Yellowcot file: [Chemical\_Elements\_1.0.yc](http://yellowcot.googlecode.com/files/Chemical_Elements_1.0.yc)

### [Revision 0](https://code.google.com/p/yellowcot/source/detail?r=0).4 ###

Per [revision 0](https://code.google.com/p/yellowcot/source/detail?r=0).4, a Yellowcot file is an uncompressed tarball including an _index.xml_ and a _media_ directory. The _media_ directory stores audio, image, and video files numbered sequentially. The _index.xml_ file stores the questions and answers that make up the flash card set. An example _index.xml_ file is shown below. The markup defines two question and answer pairs.

```
<?xml version="1.0"?>
<quiz>
	<qa>
		<q type="media">1</q>
		<qsrc>http://commons.wikimedia.org/wiki/File:Big-eared-townsend-fledermaus.jpg</qsrc>
		<qlic>PD</qlic>
		<a type="text">leðurblaka (fs)</a>
		<asrc></asrc>
		<alic></alic>
	</qa>
	<qa>
		<q type="media">2</q>
		<qsrc>http://commons.wikimedia.org/wiki/File:Female_pair.jpg</qsrc>
		<qlic>CC-BY-SA 2.0</qlic>
		<a type="text">hænsnakjöt (ns) / hænsni (ns) / kjúklingur (ms)</a>
		<asrc></asrc>
		<alic></alic>
	</qa>
</quiz>
```

Supported by Yellowcot versions: 1.2.0 - 1.2.3
<br>Backward compatibility with Yellowcot versions: 1.2.4 - 1.2.5<br>
<br>
Example (<a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.4) Yellowcot file: <a href='http://yellowcot.googlecode.com/files/Chemical_Elements_0.4.yc'>Chemical_Elements_0.4.yc</a>

<h3><a href='https://code.google.com/p/yellowcot/source/detail?r=0'>Revision 0</a>.3</h3>

Per <a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.3, a Yellowcot file is an uncompressed tarball including an <i>index.xml</i> and directories to store audio, image, and video files. The <i>index.xml</i> file stores the questions and answers that make up the flash card set. An example <i>index.xml</i> file is shown below. The markup defines two question and answer pairs.<br>
<br>
<pre><code>&lt;?xml version="1.0"?&gt;<br>
&lt;quiz&gt;<br>
	&lt;qa&gt;<br>
		&lt;q type="image"&gt;bat.jpg&lt;/q&gt;<br>
		&lt;qsrc&gt;http://commons.wikimedia.org/wiki/File:Big-eared-townsend-fledermaus.jpg&lt;/qsrc&gt;<br>
		&lt;qlic&gt;PD&lt;/qlic&gt;<br>
		&lt;a type="text"&gt;leðurblaka (fs)&lt;/a&gt;<br>
		&lt;asrc&gt;&lt;/asrc&gt;<br>
		&lt;alic&gt;&lt;/alic&gt;<br>
	&lt;/qa&gt;<br>
	&lt;qa&gt;<br>
		&lt;q type="image"&gt;chicken.jpg&lt;/q&gt;<br>
		&lt;qsrc&gt;http://commons.wikimedia.org/wiki/File:Female_pair.jpg&lt;/qsrc&gt;<br>
		&lt;qlic&gt;CC-BY-SA 2.0&lt;/qlic&gt;<br>
		&lt;a type="text"&gt;hænsnakjöt (ns) / hænsni (ns) / kjúklingur (ms)&lt;/a&gt;<br>
		&lt;asrc&gt;&lt;/asrc&gt;<br>
		&lt;alic&gt;&lt;/alic&gt;<br>
	&lt;/qa&gt;<br>
&lt;/quiz&gt;<br>
</code></pre>

Supported by Yellowcot versions: 1.1.15 - 1.1.21<br>
<br>
Example (<a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.3) Yellowcot file: <a href='http://yellowcot.googlecode.com/files/Chemical_Elements_0.3.yc'>Chemical_Elements_0.3.yc</a>

<h3><a href='https://code.google.com/p/yellowcot/source/detail?r=0'>Revision 0</a>.2</h3>

Per <a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.2, a Yellowcot file is an uncompressed tarball including an <i>index.xml</i> and directories to store audio, image, and video files. The <i>index.xml</i> file stores the questions and answers that make up the flash card set. An example <i>index.xml</i> file is shown below. The markup defines two question and answer pairs.<br>
<br>
<pre><code>&lt;?xml version="1.0"?&gt;<br>
&lt;quiz&gt;<br>
	&lt;qa&gt;<br>
		&lt;q type="image"&gt;bat.jpg&lt;/q&gt;<br>
		&lt;a type="text"&gt;leðurblaka (fs)&lt;/a&gt;<br>
	&lt;/qa&gt;<br>
	&lt;qa&gt;<br>
		&lt;q type="image"&gt;chicken.jpg&lt;/q&gt;<br>
		&lt;a type="text"&gt;hænsnakjöt (ns) / hænsni (ns) / kjúklingur (ms)&lt;/a&gt;<br>
	&lt;/qa&gt;<br>
&lt;/quiz&gt;<br>
</code></pre>

Supported by Yellowcot versions: 1.1.0 - 1.1.14<br>
<br>
Example (<a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.2) Yellowcot file: <a href='http://yellowcot.googlecode.com/files/Chemical_Elements_0.2.yc'>Chemical_Elements_0.2.yc</a>

<h3><a href='https://code.google.com/p/yellowcot/source/detail?r=0'>Revision 0</a>.1</h3>

Per <a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.1, a Yellowcot file is a simple text file. Each odd-numbered line is a question; each even-numbered line is an answer. An example file is shown below. The markup defines two question and answer pairs.<br>
<br>
<pre><code>bat<br>
leðurblaka (fs)<br>
chicken<br>
hænsnakjöt (ns) / hænsni (ns) / kjúklingur (ms)<br>
</code></pre>

Supported by Yellowcot versions: 1.0.0 - 1.0.1<br>
<br>
Example (<a href='https://code.google.com/p/yellowcot/source/detail?r=0'>revision 0</a>.1) Yellowcot file: <a href='http://yellowcot.googlecode.com/files/Chemical_Elements_0.1.yc'>Chemical_Elements_0.1.yc</a>