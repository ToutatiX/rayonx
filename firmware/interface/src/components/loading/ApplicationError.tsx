import { FC } from 'react';

interface ApplicationErrorProps {
  message?: string;
}

const ApplicationError: FC<ApplicationErrorProps> = ({ message }) => (
	<div>
		Failed to configure the application, please refresh to try again. <br/> {
		message && (<p>{message}</p>)}
	</div>
);

export default ApplicationError;
