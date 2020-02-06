#!/usr/bin/perl

local ($buffer, @pairs, $pair, $name, $value, %FORM);
# Read in text
$ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;

#check reuest method
if ($ENV{'REQUEST_METHOD'} eq "POST") { 
   $buffer = $ENV{'DATA'};
   $val1 = 'post read success<br>';
   $val2 = '<br>';
}elsif($ENV{'REQUEST_METHOD'} eq "GET"){
   $buffer = $ENV{'QUERY_STRING'};
   $val1 = 'get read success';
}else{
   $val1 = 'ERROR NO REQUEST DETECTED';	
}

# Split information into name/value pairs
@pairs = split(/&/, $buffer);

foreach $pair (@pairs) {
   ($name, $value) = split(/=/, $pair);
   $value =~ tr/+/ /;
   $value =~ s/%(..)/pack("C", hex($1))/eg;
   $FORM{$name} = $value;
}

$first_name = $FORM{first_name};
$last_name  = $FORM{last_name};

print "HTTP/1.0 200 Ok\r\n";
print "Content-type:text/html\r\n\r\n";
print "<html>";
print "<head>";
print "<title>This is the CGI program </title>";
print "</head>";
print "<body>";
print "<h2>$val1 hello $val2 $first_name $val2 $last_name </h2>";
print "</body>";
print "</html>";

1;
