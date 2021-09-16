function getData(){
  // data[0] = term_price; data[1] = months; data[2] = interest; data[3] = final_price;
  var data = document.getElementById("data").elements;
  var data_array = [];
  for(var i = 0; i < data.length-3; i++)
  {
    data_array.push(Number(data[i].value));
  }
  data_array.push(Number(document.querySelector("input[name=down_payment]:checked").value));
  return data_array;
}

function getFinancialRatio(m, t){
  return t/(1-Math.pow(1+t,-m));
}

function getInstallment(vi, m, t, d){
  var fr = getFinancialRatio(m, t)
  document.getElementById("summary").innerHTML = "<br/>Coeficiente de Rendimento: " + fr.toFixed(6);
  if(d == 1){
    return (vi*fr)/(1+t);
  }
  return vi*fr;
}

function getInterestPay(t, db){
    return t * db;
}

function getAmortization(ins, tp){
  return ins - tp;
}

function getDueBalance(db, a){
  return db - a;
}

function getInterest(vi, vf, m, d, t1, t0, i){
  //valor_final = x | valor_inicial = y
  if((Math.abs(t1 - t0) < Math.pow(10,-4)) && (i > 0)){
    return t1;
  }
  if(i == 0){
    t1 = vf/vi;
  }
  if(d == 1){
    var a = Math.pow(1+t1, m-2);
    var b = Math.pow(1+t1, m-1);
    var c = Math.pow(1+t1, m);

    var ft = (vi*t1*b) - (vf*(c-1)/m);
    var ft_ = vi*(b + t1*(m-1)*a) - vf*b;
  }
  else{
    var a = Math.pow(1+t1, -m);
    var b = Math.pow(1+t1, -m-1);

    var ft = vi*t1 - (vf/m)*(1-a);
    var ft_ = vi - vf*b;
  }
  return getInterest(vi, vf, m, d, (t1-ft/ft_), t1, i+1)
}

function price(){
  var tb = document.createElement("table");
  tb.class = ".table";
  //tb.style.width = "50%";
  //tb.setAttribute("border","1");

  var categories = ["Mês", "Prestação", "Juros", "Amortização", "Saldo Devedor"];
  // data[0] = term_price; data[1] = months; data[2] = interest; data[3] = final_price; data[4] = down_payment;
  var data = getData();

  if(data[2] == 0){
    data[2] = getInterest(data[0], data[3], data[1], data[4], 0, 0, 0);
  }else{
    data[2] = data[2]/100;
  }
  var installment = getInstallment(data[0], data[1], data[2], data[4]);
  if(data[4] == 1){
    data[1] = data[1] - 1;
    data[0] = data[0] - installment;
  }
  document.getElementById("summary").innerHTML += "<br/>TAXA DE JUROS: " + data[2] + "<br/>PARCELA: $" + installment;

  var total_interest_paid = 0;
  var total_amortization = 0;

  var thead = document.createElement("thead");
  var tbody = document.createElement("tbody");
  for(var row = 0; row <= data[1]; row++){
    var row_element = document.createElement("tr");
    if(row == 0){
      for(var c = 0; c < categories.length; c++){
        var th = document.createElement("th")
        th.appendChild(document.createTextNode(categories[c]));
        row_element.appendChild(th);
      }
      thead.appendChild(row_element);
    }else{
      for(var c = 1; c <= categories.length; c++){
        var tdata = document.createElement("td");

        var interest_paid = getInterestPay(data[2], data[0]);
        var amortization = getAmortization(installment, interest_paid);
        //TABELA
        switch(c){
          case 1:
            tdata.appendChild(document.createTextNode(row));
            break;
          case 2:
            tdata.appendChild(document.createTextNode(installment.toFixed(2)));
            break;
          case 3:
            total_interest_paid += interest_paid;
            tdata.appendChild(document.createTextNode(interest_paid.toFixed(2)));
            break;
          case 4:
            total_amortization += amortization;
            tdata.appendChild(document.createTextNode(amortization.toFixed(2)));
            break;
          case 5:
            data[0] -= amortization;
            tdata.appendChild(document.createTextNode(data[0].toFixed(2)));
            break;
        }
        row_element.appendChild(tdata);
      }
      tbody.appendChild(row_element);
    }
  }
  //CONCLUSAO
  var row_element = document.createElement("tr");
  for(var c = 1; c <= categories.length; c++){
    var tdata = document.createElement("td");
    switch(c){
      case 1:
        tdata.appendChild(document.createTextNode("Total"));
        break;
      case 2:
        tdata.appendChild(document.createTextNode((installment*data[1]).toFixed(2)));
        break;
      case 3:
        tdata.appendChild(document.createTextNode(total_interest_paid.toFixed(2)));
        break;
      case 4:
        tdata.appendChild(document.createTextNode(total_amortization.toFixed(2)));
        break;
      case 5:
        tdata.appendChild(document.createTextNode(data[0].toFixed(2)));
        break;
    }
  row_element.appendChild(tdata);
  }
  tbody.appendChild(row_element);
  tb.appendChild(thead);
  tb.appendChild(tbody);

  if(document.getElementById("table_show").hasChildNodes()){
    document.getElementById("table_show").replaceChild(tb, document.getElementById("table_show").childNodes[0]);
  }
  else{
    document.getElementById("table_show").appendChild(tb);
  }
  tb.setAttribute("class", "table table-bordered table-hover table-condensed");
  return null;
}
