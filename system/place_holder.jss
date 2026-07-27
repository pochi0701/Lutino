<?
  const w = (_GET.w == null)?300:_GET.w;
  const h = (_GET.h == null)?200:_GET.h;
  header("Content-Type: image/svg+xml");

  const svg = `
    <svg width="`+w+`" height="`+h+`" xmlns="http://www.w3.org/2000/svg">
      <rect width="100%" height="100%" fill="#ddd"/>
      <text x="50%" y="50%" dominant-baseline="middle" text-anchor="middle"
            fill="#555" font-size="20">
        `+w+`x`+h+`
      </text>
    </svg>
  `;
print(svg);
?>