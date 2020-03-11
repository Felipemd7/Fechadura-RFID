<?php
// Conexão
include_once 'php_action/db_connect.php';
// Header
include_once 'includes/header.php';
// Message
include_once 'includes/message.php';

?>
<div class="row">
	<div class="col s12 m8 push-m2">
		<h3 class="light"> Acessos </h3>
		<table class="striped">
			<thead>
				<tr>
					
					
					<th>Data</th>
					<th>Cartão</th>
				</tr>
			</thead>

			<tbody>
				<?php
				$sql = "SELECT * FROM CONTROLE.tags";
				$resultado = mysqli_query($connect, $sql);
               
                if(mysqli_num_rows($resultado) > 0):

				while($dados = mysqli_fetch_array($resultado)):
				?>
				<tr>
					<td><?php echo $dados['data']; ?></td>
					<td><?php echo $dados['cartao']; ?></td>
					

					      </form>

					    </div>
					  </div>


				</tr>
			   <?php 
				endwhile;
				else: ?>

				<tr>
					<td>-</td>
					<td>-</td>
					<td>-</td>
					<td>-</td>
				</tr>

			   <?php 
				endif;
			   ?>

			</tbody>
		</table>
		<br>
		
	</div>
</div>

<?php
// Footer
include_once 'includes/footer.php';
?>

