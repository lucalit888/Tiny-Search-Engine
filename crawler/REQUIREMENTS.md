# CS50 Lab 4 - Crawler - REQUIREMENTS.md 
## Luca Lit Feb 16 2020

### TSE Crawler:

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

### Requirements for the TSE Crawler

The crawler shall:

1. execute from a command line with usage syntax

	-> ./crawler seedURL pageDirectory maxDepth
	-> where seedURL is used as the initial URL (must be an internal URL),
	-> where pageDirectory is the pathname for an existing directory in which to write downloaded webpages, and
	-> where maxDepth is a non-negative integer representing the maximum crawl depth (a maximal allowable value for maxDepth can be set in the code)

2. crawl all pages reachable from seedURL, following links to a maximum depth of maxDepth; where maxDepth=0 means that crawler only explores the page at seedURL, maxDepth=1 means that crawler only explores the page at seedURL and those pages to which seedURL links, and so forth inductively.

3. pause at least one second between page fetches.
upon any error in fetching a page, log the error and continue to the next URL.
ignore URLs that are not “internal” (meaning, outside the designated CS50 server).

4. write each explored page to the pageDirectory with a unique document ID, wherein:
	-> the document id starts at 1 and increments by 1 for each new page,
	-> and the filename is of form pageDirectory/id,
	-> and the first line of the file is the URL,
	-> and the second line of the file is the depth,
	-> and the rest of the file is the page content (the HTML, unchanged).
	-> upon any error in writing the page to pageDirectory, log the error and continue.
