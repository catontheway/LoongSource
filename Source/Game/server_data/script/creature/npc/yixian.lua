--NPCҽ�ɽű�����ID��4001056
--��������

--�������˵Ļ�����Ǯ����
c4001056_BaseCost = {[1]    =   1,          [2]     =   15,         [3]     =   60,
                     [4]    =   135,        [5]     =   248,        [6]     =   400,
                     [7]    =   600,        [8]     =   870,        [9]		=   1200,
                     [10]	=   1650,       [11]    =   2100,       [12]	=   2750,
                     [13]	=   3500,       [14]    =   4350,       [15]    =   5330,
                     [16]	=   6450,       [17]    =   7680,       [18]	=   9200,
                     [19]	=   10950,      [20]	=   12850,      [21]	=   15000,
                     [22]	=   17300,      [23]	=   19850,      [24]	=   22650,
                     [25]	=   25650,      [26]	=   29300,      [27]	=   33200,
                     [28]	=   37440,      [29]	=   42000,      [30]	=   46950,
                     [31]	=   52250,
                    }

--ҽ�ɶԻ��ű�
function c4001056_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --����ǵ�һ�ζԻ�
	if TalkIndex == -1 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Injury"], 1)
	end

	local stepindex = role.GetRoleScriptData(RoleID, 1, RoleDataType["Injury"])

	-- �õ�������
	local injure	= role.GetRoleAttValue(MapID, InstanceID, RoleID, 64)
	local moneycur	= role.GetRoleSilver(MapID, InstanceID, RoleID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	
	-- ����������������Ҫ�Ľ�Ǯ
	local k = math.floor(rolelevel/5)+1
	local moneyneed	= c4001056_BaseCost[k] * injure
	
	-- ��һ��
	if stepindex == 1 then -- �жϽ�ɫ���������Ƿ�Ϊ0��
		local MsgID = msg.BeginMsgEvent()
		
		if injure == 0 then --��Ϊ0���򵯳��Ի�����ʾ����������Ҫ�����κ����ơ���
			msg.AddMsgEvent(MsgID, 20, 	18)			--��������Ҫ�����κ����ơ�
			msg.AddMsgEvent(MsgID, 21, 	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 	21)			--��ȷ����
			
			stepindex = 1                           --���ֵ�һ��
			
		else --����Ϊ0���������ƽ��棬��������ʾ��ɫ��ǰ����ֵ����������Լ������ơ��͡�ȡ����������ť
			msg.AddMsgEvent(MsgID, 20,	19)			--����������Ϊ####����Ҫ####��####����
			msg.AddMsgEvent(MsgID, 9,	injure)		--����
			msg.AddMsgEvent(MsgID, 9,	moneyneed / 10000)	--��
			msg.AddMsgEvent(MsgID, 9,	moneyneed % 10000)	--��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			
			stepindex = 2                           --ǰ�����ڶ���
		end
		
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		
	-- �ڶ���
	elseif stepindex == 2 then
	
		if TalkIndex == 4 then                      --ȷ��
			-- �жϽ�ɫ��Ǯ�Ƿ��㹻	        
			if moneyneed > moneycur then            -- �����㣬����ʾ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 20) 		--�����Ľ�Ǯ���㣬�޷����ơ�
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			-- ���㹻�������Ƴɹ���
			else
				-- ϵͳ�۳���Ӧ��Ǯ
				role.DecRoleSilver(MapID, InstanceID, RoleID, moneyneed, 452)
				-- ������ɫ�������Իָ�Ϊ0
				role.ModRoleAttValue(MapID, InstanceID, RoleID, 64, -injure)
			end
		end
		stepindex = 1
	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["Injury"], stepindex)
end

-- ע��
aux.RegisterCreatureEvent(4001056, 7, "c4001056_OnTalk")
