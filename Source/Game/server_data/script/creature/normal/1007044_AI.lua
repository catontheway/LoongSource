--���ﺰ���ű�

-- �������ս������
function c1007044_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40095)
       end	
end

aux.RegisterCreatureEvent(1007044, 2, "c1007044_OnEnterCombat")

