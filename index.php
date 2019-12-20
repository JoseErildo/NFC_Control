    <!DOCTYPE html>
    <html lang="pt-br">
      <head>
        <title>NodemMCU e MySQL</title>
        <meta charset="utf-8">

	<!--design-->
	<style>
		/*ESTILOS GERAIS*/
		.container{
			width: 50%;
			margin: 0 auto;
			background: #87CEEB;
		}

		/* ESTILOS FORMULARIO*/

		.areaPesquisa{
			border-radius: 5px;   //radio da borda
			background-color: #4682B4;  //fundo
			padding: 10px; //preenchimento
		}

		input {
			padding: 10px;
			margin: 8px 0;
			border: 1px solid #000;
			border-radius: 4px;
		}

		input[type=text] {
			width: 30%;
		}

		input[type=submit] {
			width: 10%;
			cursor: pointer;
		}
		/*ESTILOS TABELA*/
		
		table{
			width: 100%;
			margin-top: 10px;
		}	

		table th{
			border-collapse: collapse;
			background-color: #FF00FF;
			height: 30px;
		}	

	</style>

      </head>
      <body>
	<div class="container">

	<div class= "areaPesquisa">
        	<form action="" method="POST">
			<input type="text" name="data" placeholder=" estilo de busca: 'mês/ano'">
			<input type="submit" name="submit" value="Buscar">
		</form>
	</div>

	<?php
		include('conexao.php');

		if($_SERVER['REQUEST_METHOD'] == "POST"){
			
			$dataPesquisa = $_POST['data'];

			//Convenção de pesquisa MM/AA ex: 07/12
			// '1' armazena o ano, '0' armazena o mes
			$dataArray = explode("/", $dataPesquisa);
			$dataPesquisa = $dataArray[1] . "-" . $dataArray[0];

			$sql = "SELECT * FROM nfc_data WHERE date_hora LIKE '%" . $dataPesquisa . "%'"; //LIKE '%2017-09-15%'
		} else{
			
			$dataAtual = date('Y-m');

			echo "Mes atual:" . $dataAtual;
			$sql = "SELECT * FROM nfc_data WHERE date_hora LIKE '%" . $dataAtual . "%'";
		}

		$stmt = $PDO->prepare($sql);
		$stmt->execute();

		echo "<table border=\"1\">";

		echo "<tr>
			<th>MAC</th>
			<th>NFC</th>
			<th>Data/Hora</th>
		</tr>";

		while($linha = $stmt->fetch(PDO::FETCH_OBJ)){
			
			$timestamp = strtotime($linha->date_hora);

			//Formatando data e hora para convenção brasileira: DD/MM/AA
			//H maiúsculo para formato de horário de 24horas
			//i = minutos
			//s = segundos
			$dataTabela = date('d/m/Y H:i:s', $timestamp);

			echo "<tr>";
			echo "<td>" . $linha->mac . "</td>";
			echo "<td>" . $linha->nfc . "</td>";
			echo "<td>" . $dataTabela . "</td>"; //Data e hora formatada
			echo "</tr>";
		}

		echo "</table>";
	?>

	</div>
      </body>
    </html>