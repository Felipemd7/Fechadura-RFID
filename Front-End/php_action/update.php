<?php
// Sessão
session_start();
// Conexão
require_once 'db_connect.php';

if(isset($_POST['btn-editar'])):
	$nome = mysqli_escape_string($connect, $_POST['nome']);
	$cartao = mysqli_escape_string($connect, $_POST['cartao']);
	$setor = mysqli_escape_string($connect, $_POST['setor']);
	
	$id = mysqli_escape_string($connect, $_POST['id']);

	$sql = "UPDATE cadastrados SET nome = '$nome', cartao = '$cartao', setor = '$setor' WHERE id = '$id'";

	if(mysqli_query($connect, $sql)):
		$_SESSION['mensagem'] = "Atualizado com sucesso!";
		header('Location: ../index.php');
	else:
		$_SESSION['mensagem'] = "Erro ao atualizar";
		header('Location: ../index.php');
	endif;
endif;