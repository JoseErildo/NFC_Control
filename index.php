    <!DOCTYPE html>
    <html lang="pt-br">
      <head>
        <title>NodemMCU e MySQL</title>
        <meta charset="utf-8">

	//design
	<style>
		/*ESTILOS GERAIS*/

		

		/* ESTILOS FORMULARIO*/

		/*ESTILOS TABELA*/
	</style>
      </head>
      <body>
	<div class="">
        <form action="" method="POST">
		<input type="text" name="data">
		<input type="submit" name="submit" value="Buscar">
	</form>

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