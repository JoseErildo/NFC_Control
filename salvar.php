<?php

	include('conexao.php');

	$mac = $_GET['mac'];
	$nfc = $_GET['nfc'];

	$sql = "INSERT INTO nfc_data (mac, nfc) VALUES (:mac, :nfc)";

	$stmt = $PDO->prepare($sql);

	$stmt->bindParam(':mac', $mac);
	$stmt->bindParam(':nfc', $nfc);

	if($stmt->execute()){
		echo "salvo_com_sucesso";
	} else {
		echo "erro_ao_salvar";
	}
?>