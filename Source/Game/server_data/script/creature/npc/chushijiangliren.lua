--NPC��ʦ�����˽ű� ID:4900302
--ʦ������ͽ����ȡ��ʦ������ʦ����ȡ���佱��

--NPC��ʦ�����˽ű��Ի��ű�
function c4900302_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

  --����ǵ�һ�ζԻ�
	if TalkIndex == -1 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ChuShiJiangLi"], 1)
	end

	local stepindex = role.GetRoleScriptData(RoleID, 1, RoleDataType["ChuShiJiangLi"])

	if stepindex == 1 then

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 20, 	181)			--������������ͽ��<color=0xffff0000>��ʦ<color=0xfffff7e0>����������������ȡ�����������ͽ��ʹ��<color=0xffff0000>�������<color=0xfffff7e0>�󣬻�Ϊʦ������<color=0xffff0000>�������<color=0xfffff7e0>ֵ��ʦ����ʹ����������ڴ˴���ȡ���佱������ȡһ����Ҫ�����������<color=0xffff0000>3000<color=0xfffff7e0>�㣡��
		msg.AddMsgEvent(MsgID, 24, 	TargetID)	    --npc id
		msg.AddMsgEvent(MsgID, 21, 	8)			    --��ʦ����ȡ��ʦ��������ť
		msg.AddMsgEvent(MsgID, 1, 	182)			--��ʦ����ȡ��ʦ�������ı�
		msg.AddMsgEvent(MsgID, 21, 	9)			    --��ͽ����ȡ��ʦ��������ť
		msg.AddMsgEvent(MsgID, 1, 	183)			--��ͽ����ȡ��ʦ�������ı�
		msg.AddMsgEvent(MsgID, 21, 	10)			    --��ʦ����ȡʦ�½�������ť
		msg.AddMsgEvent(MsgID, 1, 	184)			--��ʦ����ȡʦ�½������ı�
		msg.AddMsgEvent(MsgID, 21, 	11)			    --����ȡͽ��������������ť
		msg.AddMsgEvent(MsgID, 1, 	196)			--����ȡͽ�������������ı�

		stepindex = 2                       --ǰ�����ڶ���

		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ

	elseif stepindex == 2 then

		local BagFreeSize = role.GetBagFreeSize(RoleID)

		if TalkIndex == 8 then							--��ʦ����ȡ��ʦ��������ť

			local MsgID = msg.BeginMsgEvent()
			local RemindID = 0

			--ʦ�����ó�ʦ��������
			local nRewardNum = role.GetGraduateRewardNum(MapID, InstanceID, RoleID)

			if nRewardNum < 1 then 				--����ң��ѳ�ʦ��������-��ȡ���ӳ�ʦ��������=0��

				RemindID = 186			--��ʾ������û���ѳ�ʦ�ĵ��ӣ������Ѿ���ȡ�����ӳ�ʦ��������

			else								--ͨ�������жϺ���һ�ü�ָ��������

				--ʦ�����ó�ʦ����������1
				nRewardNum = nRewardNum - 1
				role.SetGraduateRewardNum(MapID, InstanceID, RoleID, nRewardNum)

				--��þ���
				role.AddRoleExp(MapID, InstanceID, RoleID, 200000)

				--��ʾ�������ѳɹ���ȡ���ӳ�ʦ�Ľ�������
				RemindID = 188

				--ȫ������ƹ㲥��
				local ServerMsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(ServerMsgID, 100, 189)				        			--��<ʦ����>����ͽ������50����ʦ�����20��㾭��ֵ�Ľ�������
				msg.AddMsgEvent(ServerMsgID, 2, RoleID)		--<ʦ����>
				msg.DispatchWorldMsgEvent(ServerMsgID)
			end

			msg.AddMsgEvent(MsgID, 20, RemindID)			--��ʾ����
			msg.AddMsgEvent(MsgID, 21, 4)							--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 185)						--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)   --������Ϣ

		elseif TalkIndex == 9 then					--��ͽ����ȡ��ʦ��������ť

			local MsgID = msg.BeginMsgEvent()
			local RemindID = 0

			local GraduateReward = role.GetRoleScriptData(RoleID, 1, RoleDataType["GraduateReward"])

			if GraduateReward ~= 1 then						--�������δ��ʦ�����������ȡ����ʦ����

				RemindID = 190			--��ʾ��������δ��ʦ��������ȡ����ʦ��������

			elseif BagFreeSize < 1 then		--��������ҿո���<1

				RemindID = 187			--��ʾ�������ҿռ䲻�㣬��������ٽ�����ȡ����

			else							--ͨ�������жϺ���һ�ü�ָ��������

			  --��ʾ�������ȡ��ͽ�ܳ�ʦ����
				role.SetRoleScriptData(RoleID, 1, RoleDataType["GraduateReward"], 0)

				--��þ���
				role.AddRoleExp(MapID, InstanceID, RoleID, 1000000)

				--���ʮ����ɱͼ�Ľ���
				role.AddRoleItem(MapID, InstanceID, RoleID, 1350001, 10, 3, 104)

				--��ʾ�������ѳɹ���ȡ��ʦ�Ľ�������
				RemindID = 191

				--ȫ������ƹ㲥��
				local ServerMsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(ServerMsgID, 100, 192)				        --"<ͽ����>��������50����ʦ�������100��㾭��ֵ�Ľ���������ʦ��ſɻ�ó�ʦ��������"
				msg.AddMsgEvent(ServerMsgID, 2, RoleID)		--<ͽ����>
				msg.DispatchWorldMsgEvent(ServerMsgID)

			end

			msg.AddMsgEvent(MsgID, 20, RemindID)			--��ʾ����
			msg.AddMsgEvent(MsgID, 21, 4)							--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 185)						--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)   --������Ϣ

		elseif TalkIndex == 10 then					--��ʦ����ȡ���佱������ť

			local MsgID = msg.BeginMsgEvent()
			local RemindID = 0
			local QinWuPoint = role.GetQinWuPoint(MapID, InstanceID, RoleID)

			if QinWuPoint < 3000 then				--�������������3000

				RemindID = 193			--��ʾ�������������������3000�㣬�޷���ȡ��������

			elseif BagFreeSize < 4 then			--��������ҿո���<4

				RemindID = 187			--��ʾ�������ҿռ䲻�㣬��������ٽ�����ȡ����

			else								    				--ͨ�������жϺ���һ�ü�ָ��������

				--�۳�ʦ�µ���
				QinWuPoint = QinWuPoint - 3000
				role.SetQinWuPoint(MapID, InstanceID, RoleID, QinWuPoint)

				--���һ�����������������������׺ϳɷ�Ҽ���Ľ���
				role.AddRoleItem(MapID, InstanceID, RoleID, 1300100, 1, 3, 104)
				--ע�⣬��Ϊ�����׺ϳɷ�Ҽ���������Ϊ1��һ�δ���������Ʒ�ᵼ��ʧ�ܣ�����ÿ��ֻ����ȡһ����
				role.AddRoleItem(MapID, InstanceID, RoleID, 1412004, 1, 3, 104)
				role.AddRoleItem(MapID, InstanceID, RoleID, 1412004, 1, 3, 104)
				role.AddRoleItem(MapID, InstanceID, RoleID, 1412004, 1, 3, 104)

				--��ʾ�������ѳɹ���ȡ���佱������
				RemindID = 194

				--ȫ������ƹ㲥��
				local ServerMsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(ServerMsgID, 100, 195)				        			--"<ʦ����>ʹ��3000�����������ȡ�˻����������׺ϳɷ�Ҽ�Ľ�����"
				msg.AddMsgEvent(ServerMsgID, 2, RoleID)		--<ʦ����>
				msg.DispatchWorldMsgEvent(ServerMsgID)

			end

			msg.AddMsgEvent(MsgID, 20, RemindID)			--��ʾ����
			msg.AddMsgEvent(MsgID, 21, 4)							--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 185)						--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)   --������Ϣ

		elseif TalkIndex == 11 then                 --"��ȡͽ����������"��ť

			local MsgID = msg.BeginMsgEvent()
			local RemindID = 0
			local LevelUpBonus = role.GetLevelUpBonus(MapID, InstanceID, RoleID)

			if LevelUpBonus == nil then
				LevelUpBonus = 0
			end

			if LevelUpBonus < 350 then                        --��ͽ���������ֲ���350

				--��ʾ��"����ǰ��ͽ����������Ϊ<ͽ����������>�㣬��Ҫ�ﵽ350��ſ���ȡͽ���������ֽ�����"
				RemindID = 197;

			elseif BagFreeSize < 2 then                    --��������ҿո���<2

				--��ʾ��"���ҿռ䲻�㣬��������ٽ�����ȡ��"
				RemindID = 187

			else                            --ͨ�������жϺ���һ�ü�ָ������

				--��������Ʒ
				local ItemID = {1411003, 1411004, 1411005}
				local ItemID2 = {4200008, 4200009, 4200010}
				local ItemID3 = {4200108, 4200109, 4200110}
				local ItemID4 = {4200208, 4200209, 4200210}
				local ItemID5 = {4200308, 4200309, 4200310}
				local ItemID6 = {4200408, 4200409, 4200410}
				local ItemID7 = {4200508, 4200509, 4200510}
				local ItemName = {"����֮���", "����֮���", "����֮���"}
				local ItemNum = {3, 2, 1}

				local temp = math.random(100)
				local RandNum = 0

				if temp<=60 then
					RandNum = 1
				elseif temp<=92 then
					RandNum = 2
				else
					RandNum = 3
				end

				temp = math.random(12)
				local ItemID_give = 0
				local ItemNum_give = 0

				if temp<=6 then
					ItemID_give = ItemID[RandNum]
					ItemNum_give = ItemNum[RandNum]
				elseif temp==7 then
					ItemID_give = ItemID2[RandNum]
					ItemNum_give = 1
				elseif temp==8 then
					ItemID_give = ItemID3[RandNum]
					ItemNum_give = 1
				elseif temp==9 then
					ItemID_give = ItemID4[RandNum]
					ItemNum_give = 1
				elseif temp==10 then
					ItemID_give = ItemID5[RandNum]
					ItemNum_give = 1
				elseif temp==11 then
					ItemID_give = ItemID6[RandNum]
					ItemNum_give = 1
				elseif temp==12 then
					ItemID_give = ItemID7[RandNum]
					ItemNum_give = 1
				end

				role.AddRoleItem(MapID, InstanceID, RoleID, ItemID_give, ItemNum_give, 3, 104)

				--���5�����񽳾���
				role.AddRoleItem(MapID, InstanceID, RoleID, 2005201, 5, 3, 104)

				--��ʾ��"���ѳɹ���ȡͽ���������ֽ�����"
				RemindID = 198

				--ȫ������ƹ㲥��
				local ServerMsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(ServerMsgID, 100, 199)												--ȫ������ƹ㲥��"<ʦ����>ʹ��350��ͽ���������ֻ�ȡ��<���������>���񽳾��Ľ�����"
				msg.AddMsgEvent(ServerMsgID, 2, RoleID)			--<ʦ����>
				msg.AddMsgEvent(ServerMsgID, 4, ItemID_give)     				--<���������>
				msg.DispatchWorldMsgEvent(ServerMsgID)

				role.SetLevelUpBonus(MapID, InstanceID, RoleID, LevelUpBonus - 350)

			end

			msg.AddMsgEvent(MsgID, 20, RemindID)			--��ʾ����
    		if RemindID == 197 then
				msg.AddMsgEvent(MsgID, 9, LevelUpBonus)   --<ͽ����������>
			end
			msg.AddMsgEvent(MsgID, 21, 4)							--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 185)						--��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)   --������Ϣ

		end

		stepindex = 1			--�ص���һ��

	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["ChuShiJiangLi"], stepindex)

end

--ע���ʦ������NPC�Ի��¼�
aux.RegisterCreatureEvent(4900302, 7, "c4900302_OnTalk")
