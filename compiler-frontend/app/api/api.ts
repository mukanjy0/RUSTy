import axios from "axios";

const BACKEND_URL = process.env.NEXT_PUBLIC_API_URL;

export const postCode = async ({ code }: { code: string }) => {
  console.log("Fetching Code to", BACKEND_URL);
  const response = await axios.post(`${BACKEND_URL}/compile`, { code });
  return response.data;
};