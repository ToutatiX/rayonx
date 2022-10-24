import { FC, useContext, useState } from 'react';
import { ValidateFieldsError } from 'async-validator';
import { useSnackbar } from 'notistack';

import * as AuthenticationApi from './api/authentication';
import { PROJECT_NAME } from './api/env';
import { SignInRequest } from './types';
import { ValidatedTextField } from './components';
import { SIGN_IN_REQUEST_VALIDATOR, validate } from './validators';
import { extractErrorMessage, onEnterCallback, updateValue } from './utils';
import { AuthenticationContext } from './contexts/authentication';

const SignIn: FC = () => {
  const authenticationContext = useContext(AuthenticationContext);
  const { enqueueSnackbar } = useSnackbar();

  const [signInRequest, setSignInRequest] = useState<SignInRequest>({
    username: '',
    password: ''
  });
  const [processing, setProcessing] = useState<boolean>(false);
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();

  const updateLoginRequestValue = updateValue(setSignInRequest);

  const validateAndSignIn = async () => {
    setProcessing(true);
    try {
      await validate(SIGN_IN_REQUEST_VALIDATOR, signInRequest);
      signIn();
    } catch (errors: any) {
      setFieldErrors(errors);
      setProcessing(false);
    }
  };

  const signIn = async () => {
    try {
      const { data: loginResponse } = await AuthenticationApi.signIn(signInRequest);
      authenticationContext.signIn(loginResponse.access_token);
    } catch (error: any) {
      if (error.response?.status === 401) {
        enqueueSnackbar("Invalid login details", { variant: "warning" });
      } else {
        enqueueSnackbar(extractErrorMessage(error, "Unexpected error, please try again"), { variant: "error" });
      }
      setProcessing(false);
    }
  };

  const submitOnEnter = onEnterCallback(signIn);

  return (
   <></>
  );
};

export default SignIn;
