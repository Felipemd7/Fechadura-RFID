<?php
// Header
include_once 'includes/header.php';
?>

<div class="row">
	<div class="col s12 m6 push-m3">
		<h3 class="light"> Novo Usuario </h3>
		<form action="php_action/create.php" method="POST">
			<div class="input-field col s12">
				<input type="text" name="nome" id="nome">
				<label for="nome">Nome</label>
			</div>

			<div class="input-field col s12">
				<input type="text" name="cartao" id="cartao">
				<label for="cartao">cartao</label>
			</div>

			<div class="input-field col s12">
				<input type="text" name="setor" id="setor">
				<label for="setor">setor</label>
			</div>

			<button type="submit" name="btn-cadastrar" class="btn"> Cadastrar </button>
			<a href="index.php" class="btn green"> Lista de Usuario </a>
		</form>
		
	</div>
</div>

<?php
// Footer
include_once 'includes/footer.php';
?>
