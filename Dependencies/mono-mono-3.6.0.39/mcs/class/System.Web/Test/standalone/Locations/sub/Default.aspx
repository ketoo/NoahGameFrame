<%@ Page Language="C#" %>
<html>
  <head>
    <title>/sub/Default.aspx</title>
  </head>
  <body>
<%= AppDomain.CurrentDomain.GetData ("BEGIN_CODE_MARKER") %>/sub/Hello<%= AppDomain.CurrentDomain.GetData ("END_CODE_MARKER") %>
  </body>
</html>

